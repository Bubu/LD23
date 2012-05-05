#include <level.h>
#include <tile.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>

Level::Level(int n,int id):_tg(TriangleGraph::TriangleGraph(n)),_size(_tg.size()),id(id),_n(n)
{
	//srand(17);
	_tiles = new Tile[_size]();
	/*for (int i=0;i<_size;i++)if (i%2==0) 
	{
		_tiles[i].color=Vector3f(1,0.5,0.3);
		_tiles[i].type=1;
	}*/

	//for (int i=0;i<_size;i++)
	//{
		//if (i%2==0) _tiles[i].color=Vector3f(0.2,0.2,0.2);
	//	//_tiles[i].type=1;
	//	_tiles[i].height=(float)rand() / (RAND_MAX + 1) * (1.15 - 0.85)  + 0.85;
	//}
	_tiles[startTile()].type=Tile::start;
	_tiles[startTile()].height=1.0f;
	
	_tiles[693 ].type=Tile::portal;	_tiles[1119].type=Tile::portal;
	_tiles[693 ].height=1.01f;		_tiles[1119].height=1.01f;
	_tiles[1103].type=Tile::portal;	_tiles[1141].type=Tile::portal;
	_tiles[1103].height=1.01f;		_tiles[1141].height=1.01f;
	_tiles[671 ].type=Tile::portal;	_tiles[655 ].type=Tile::portal;
	_tiles[671 ].height=1.01f;		_tiles[655 ].height=1.01f;
	
	
	
}

Level::~Level()
{
	delete[] _tiles;
}
static std::string getTile(Tile& tile)
{
	//enum _Type {empty=0,grass=1,mountain=2,water=3,stonewall=4,way=5,street=6,woodbridge=7,stonebridge=8,start=10,portal=11} type;
	std::string s;
	switch(tile.type)
	{
		case Tile::empty:		s="empty";break;
		case Tile::grass: 		s="grass";break;
		case Tile::mountain: 	s="mountain";break;
		case Tile::water: 		s="water";break;
		case Tile::stonewall: 	s="stonewall";break;
		case Tile::way: 		s="way";break;
		case Tile::street: 		s="street";break;
		case Tile::woodbridge: 	s="woodbridge";break;
		case Tile::stonebridge: s="stonebridge";break;
		case Tile::start: 		s="start";break;
		case Tile::portal: 		s="portal";break;
	}
	std::stringstream ss;
	ss<<tile.height;			
	s+="["+ss.str()+"]";
	return s;
}

void Level::save(std::string file)const
{
	std::ofstream out( file.c_str());
	out<<"level\n{tiles["<<_n<<"]\n{"<<getTile(_tiles[0]);
	for (int i=1;i<_size;i++)out<<",\n"<<getTile(_tiles[i]);
	out<<"\n}\n}\n";
	
}

static inline bool goToNextToken(const std::string& s, int& pointer)
{
	for (pointer;pointer<s.size()&&
	                    (s[pointer]==' '||s[pointer]=='\f'||
					     s[pointer]=='\n'||s[pointer]=='\r'||
						 s[pointer]=='\t'||s[pointer]=='\v'||s[pointer]=='#');pointer++)
		if(s[pointer]=='#')for (pointer;pointer<s.size()&& s[pointer]!='\n';pointer++);
	return (pointer==s.size());	
}

static inline std::string getToken(const std::string& s, int& pointer)
{
	std::string tmp;
	for (pointer;pointer<s.size()&&(s[pointer]=='0' || s[pointer]=='1' || s[pointer]=='2' || 
	                                s[pointer]=='3' || s[pointer]=='4' || s[pointer]=='5' ||
									s[pointer]=='6' || s[pointer]=='7' || s[pointer]=='8' ||
									s[pointer]=='9' || s[pointer]=='.');pointer++)tmp+=s[pointer];
	if (pointer==s.size())return "";
	return tmp;	
}
static inline bool getTile(const std::string& content, int& p, Tile& tile, std::string& error, bool isLast)
{
	error="";
	if(p+3>=content.size()){error="expected Tile";return false;}
	
	int t=-1;
	if (content[p+0]=='e' && content[p+1]=='m' && content[p+2]=='p')t=0;
	if (content[p+0]=='g' && content[p+1]=='r' && content[p+2]=='a')t=1;
	if (content[p+0]=='m' && content[p+1]=='o' && content[p+2]=='u')t=2;
	if (content[p+0]=='w' && content[p+1]=='a' && content[p+2]=='t')t=3;
	if (content[p+0]=='s' && content[p+1]=='t' && content[p+2]=='o')t=4;
	if (content[p+0]=='w' && content[p+1]=='a' && content[p+2]=='y')t=5;
	if (content[p+0]=='s' && content[p+1]=='t' && content[p+2]=='r')t=6;
	if (content[p+0]=='w' && content[p+1]=='o' && content[p+2]=='o')t=7;
	// (content[p+0]=='s' && content[p+1]=='r' && content[p+2]!='o')t=8;
	
	if (content[p+0]=='s' && content[p+1]=='t' && content[p+2]=='a')t=10;
	if (content[p+0]=='p' && content[p+1]=='o' && content[p+2]=='r')t=11;
	p+=3;
	if (t==4)
	{
		if(p+3>=content.size()){error="expected Tile";return false;}
		if (content[p+0]=='n' && content[p+1]=='e' && content[p+2]=='w')t=4;
		else if (content[p+0]=='n' && content[p+1]=='e' && content[p+2]=='b')t=8;
		else {error="expected Tile";return false;}
		p+=3;	
	}
	switch(t)
	{
		case 0:
		{
			if(p+3>=content.size() || content[p+0]!='t' || content[p+1]!='y' || content[p+2]!='['){error="expected Tile";return false;}
			p+=3;tile.type=Tile::empty;
		}break;	
		case 1:
		{
			if(p+3>=content.size() || content[p+0]!='s' || content[p+1]!='s' || content[p+2]!='['){error="expected Tile";return false;}
			p+=3;tile.type=Tile::grass;
		}break;	
		case 2:
		{
			if(p+6>=content.size() || content[p+0]!='n' || content[p+1]!='t' || content[p+2]!='a' || content[p+3]!='i' || content[p+4]!='n' || content[p+5]!='['){error="expected Tile";return false;}
			p+=6;tile.type=Tile::mountain;
		}break;	
		case 3:
		{
			if(p+3>=content.size() || content[p+0]!='e' || content[p+1]!='r' || content[p+2]!='['){error="expected Tile";return false;}
			p+=3;tile.type=Tile::water;
		}break;	
		case 4:
		{
			if(p+4>=content.size() || content[p+0]!='a' || content[p+1]!='l' || content[p+2]!='l' || content[p+3]!='['){error="expected Tile";return false;}
			p+=4;tile.type=Tile::stonewall;
		}break;	
		case 5:
		{
			if(p+1>=content.size() || content[p+0]!='['){error="expected Tile";return false;}
			p+=1;tile.type=Tile::way;
		}break;	
		case 6:
		{
			if(p+4>=content.size() || content[p+0]!='e' || content[p+1]!='e' || content[p+2]!='t' || content[p+3]!='['){error="expected Tile";return false;}
			p+=4;tile.type=Tile::street;
		}break;	
		case 7:
		{
			if(p+8>=content.size() || content[p+0]!='d' || content[p+1]!='b' || content[p+2]!='r' || content[p+3]!='i' || content[p+4]!='d' || content[p+5]!='g' || content[p+4]!='e' || content[p+5]!='['){error="expected Tile";return false;}
			p+=8;tile.type=Tile::woodbridge;
		}break;	
		case 8:
		{
			if(p+6>=content.size() || content[p+0]!='r' || content[p+1]!='i' || content[p+2]!='d' || content[p+3]!='g' || content[p+4]!='e' || content[p+5]!='['){error="expected Tile";return false;}
			p+=6;tile.type=Tile::stonebridge;
		}break;	
		case 10:
		{
			if(p+3>=content.size() || content[p+0]!='r' || content[p+1]!='t' || content[p+2]!='['){error="expected Tile";return false;}
			p+=3;tile.type=Tile::start;
		}break;
		case 11:
		{
			if(p+4>=content.size() || content[p+0]!='t' || content[p+1]!='a' || content[p+2]!='l' || content[p+3]!='['){error="expected Tile";return false;}
			p+=4;tile.type=Tile::portal;
		}break;	
	}
	std::string st=getToken(content,p);
	{
		float h=0.0f;
		std::stringstream ss;ss<<st;ss>>h;
		if (h<=0.0f){error="expected height>0 {"+st+"}";return false;}
		tile.height=h;	
	}
	if(p+1>=content.size() || content[p+0]!=']' ){error="expected number]";return false;}
	p++;
	if (!isLast)
	{
		if (goToNextToken(content,p)){error=",";return false;}
		if(p+1>=content.size() || content[p+0]!=',' ){error=","+content.substr(p,1);return false;}
		p+=1;
	}
	if (goToNextToken(content,p)){error="unexpected end of file";return false;}
	
}

bool Level::load(std::string file, std::string& error)
{
	std::ifstream in( file.c_str(),std::ios_base::in);
	if (!in.is_open()){error="Could not open file";return false;}
	std::string content;
	std::string s;
	char c;
	while (in.good())
	{
		c = in.get(); 
		if (in.good())content+=c;
	}
	in.close();
	int p=0;
	if (goToNextToken(content,p)){error="expected level at begin of file";return false;}
	if(p+5>=content.size() || content[p+0]!='l' || content[p+1]!='e' || content[p+2]!='v' || content[p+3]!='e' || content[p+4]!='l'){error="expected level at begin of file";return false;}
	p+=5;
	if (goToNextToken(content,p)){error="expected1 {";return false;}
	if(p+1>=content.size() || content[p+0]!='{' ){error="expected2 {";return false;}
	p+=1;
	if (goToNextToken(content,p)){error="expected tiles[n]";return false;}
	if(p+6>=content.size() || content[p+0]!='t' || content[p+1]!='i' || content[p+2]!='l' || content[p+3]!='e' || content[p+4]!='s' || content[p+5]!='['){error="expected tiles[n]";return false;}
	p+=6;
	//if (){error="expected tiles[n]";return false;}
	{
		s=getToken(content,p);
		int n=-1;
		std::stringstream ss;ss<<s;ss>>n;
		if (n!=3){error="you can only choose n=3 for tiles";return false;}	
		//_n=n;
		//_size=80*
	}
	if(p+1>=content.size() || content[p+0]!=']' ){error="expected tiles[n]";return false;}
	p++;
	if (goToNextToken(content,p)){error="expected3 {";return false;}
	if(p+1>=content.size() || content[p+0]!='{' ){error="expected4 {"+content.substr(p,1);return false;}
	p+=1;
	
	for (int i=0;i<_size-1;i++)
	{
		if (!getTile(content, p, _tiles[i], error,false))return false;
	}
	if (!getTile(content, p, _tiles[_size-1], error,true))return false;
	
	if (goToNextToken(content,p)){error="expected }";return false;}
	if(p+1>=content.size() || content[p+0]!='}' ){error="expected }";return false;}
	p+=1;
	
	if (goToNextToken(content,p)){error="expected }";return false;}
	if(p+1>=content.size() || content[p+0]!='}' ){error="expected }";return false;}
	p+=1;
	
	return true;
}
/*void Level::draw()const
{

}*/
