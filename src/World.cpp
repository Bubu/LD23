#include <World.h>
#include <iostream>
World::World():_level0(Level(3,0)),_level1(Level(3,1)),_level2(Level(3,2)),maxLevel(3),_currentLevel(0),_attack(Projectile(0.0f, 0.0f, Matrix3x3f()))
{
	std::string error;
	if (_level0.load("level0.lev",error))std::cout<<error<<std::endl;
	if (_level1.load("level1.lev",error))std::cout<<error<<std::endl;
	if (_level2.load("level2.lev",error))std::cout<<error<<std::endl;
	/*_level0[997].color= Vector3f(1,0,0); //start tile

	const int end_count = 6;
	int end[] = {693,1119,1103,1141,671,655};
	for(int i = 0; i < end_count; i++)
	{
		_level0[end[i]].type = 5; 
		_level0[end[i]].color = Vector3f(1,0,1);
		_level0[end[i]].height = 1.01;

	}

	const int mountain_count = 21+4+21+22+28+21+9+24+56+71+38+13;
	int mountain[] = {792,793,998,996,999,804,806,985,807,795,774,768,769,786,798,803,801,800,982,809,977,974,962,972,1014,
	1007,995,349,1018,1009,1010,371,374,351,333,331,321,347,992,1000,340,638,635,325,352,353,
	586,590,578,621,615,628,631,1278,341,1276,791,589,594,601,523,571,566,513,518,551,619,616,618,426,490,
	424,427,362,554,430,418,428,393,419,429,392,395,441,433,435,432,439,1143,652,654,694,692,695,1117,1107,
	516,536,507,508,509,1150,1136,1138,1137,1145,1099,1090,1088,1091,537,529,535,1194,661,440,474,913,912,915,1075,1079,925,1193,1185,1192,
	1095,1114,1092,1093,933,935,931,928,1110,1108,1109,938,297,298,296,289,294,941,905,904,906,292,
	1008,1022,381,368,369,377,328,329,1275,1266,1277,588,629,591,606,593,592,514,526,525,524,542,540,543,1199,
	1196,1195,1190,1188,1158,1191,562,573,574,572,1162,1160,1163,1209,1201,1215,575,725,724,723,727,719,757,1180,1183,1182,1165,1159,1155,1156,
	703,700,687,650,684,686,674,39,6,1,21,20,23,19,16,18,28,30,31,15,53,52,55,51,49,48,59,50,56,58,10,47,36,37,679,676,678,1177,1181,
	1171,1175,765,32,33,38,40,43,41,87,84,81,86,755,753,752,761,762,760,79,76,78,90,69,68,70,67,64,66,64,65,1232,1243,1240,1241,1233,1234,1242,1221,1220,1223,
	1255,1222,1252,1253,741,740,742,776,777,813,778,826,154,133,165,815,812,148,145,150,153,152,105,97,96,108,110,98,
	295,262,257,256,258,268,270,271,287,284,274,272,269,286,275,279,285};
	for(int i = 0; i < mountain_count; i++)
	{
		_level0[mountain[i]].type = 1; 
		_level0[mountain[i]].color = Vector3f(0.447,0.38,0.286);
		_level0[mountain[i]].height = 1.1;
		_level0[mountain[i]].blocking = true;
		if (mountain[i]%2==0) _level0[mountain[i]].color*=1.1;
	}

	const int grass_count = 5+11+16+87+14+79+39+12+43+73+146+53+245;
	int grass[] = {773,805,986,965,984,964,966,961,960,963,973,967,987,978,990,976,775,771,781,780,783,821,820,822,782,770,779,825,817,819,823,816,
	969,968,970,971,1017,1016,1019,375,372,373,335,332,334,322,320,323,327,324,326,359,356,357,623,620,358,622,610,608,611,609,555,617,546,544,557,
	547,612,521,520,522,570,602,600,603,583,579,576,577,585,584,587,
	630,625,633,348,350,338,337,342,345,344,346,634,632,339,343,1003,994,1006,1004,1005,626,624,627,637,636,639,1279,789,788,790,1001,993,785,784,
	787,336,640,641,646,644,645,666,664,665,657,663,660,662,1149,1139,
	552,545,553,491,488,548,550,494,492,494,482,480,481,489,486,484,485,431,394,442,453,452,495,549,517,538,506,458,456,459,505,504,497,496,499,503,1147,500,502,498,510,534,532,533,
	511,1151,1148,1147,1146,1144,501,463,479,1135,1098,1096,1133,1132,1134,1120,1122,477,476,478,455,475,466,464,472,473,465,443,434,446,470,468,467,471,1131,
	1129,1128,1130,469,444,447,1086,445,1084,1087,917,918,916,919,1085,1089,1094,1127,1124,1126,921,1104,1106,1125,922,920,1105,1113,901,900,903,923,914,
	1116,1101,1100,1102,1140,1142,1115,1112,932,934,929,937,936,939,942,940,943,293,
	568,569,561,560,563,564,565,527,1210,567,1211,1200,1202,1214,1212,1213,1203,1207,733,735,1204,
	1205,1167,1164,1166,1206,1154,1152,1153,1161,1197,1187,1184,1186,1198,531,541,528,530,539,519,515,794,
	1189,1157,677,1178,1176,1179,1170,1168,1169,1174,681,673,672,675,685,649,683,680,682,1173,1172,42,85,767,764,766,754,
	763,89,88,91,71,77,94,82,80,83,93,92,46,34,44,35,45,9,8,11,57,2,54,14,12,13,3,7,4,5,26,24,27,698,696,697,689,688,690,702,22,17,25,699,691,701,
	830,818,827,824,797,796,799,1269,1231,1247,1245,1244,1246,1229,1219,1228,1230,1270,1268,1271,1267,1264,1265,1273,1227,1218,1216,1272,1274,1226,1224,1225,1217,607,
	1263,1260,1261,1251,604,605,1262,1250,1248,595,599,1259,596,598,597,1258,1256,1257,1249,1254,726,721,729,728,730,709,708,710,705,704,706,718,756,758,759,715,712,713,749,
	739,743,737,736,738,747,750,748,751,714,101,100,102,104,106,103,99,107,151,147,157,109,73,72,74,111,159,156,158,146,144,155,135,
	131,141,140,181,143,122,120,75,121,113,118,116,95,117,119,115,112,114,125,124,126,123,183,179,189,180,182,177,176,185,142,130,139,128,129,134,132,136,137,173,
	29,302,290,288,291,301,300,303,266,265,264,314,312,267,313,315,306,304,305,310,308,311,307,62,60,61,318,316,319,63,127,191,255,317,254,252,242,251,248,309,250,
	239,238,236,237,201,200,188,190,253,243,240,241,249,178,187,247,244,246,184,186,245,207,138,175,223,220,221,172,174,163,167,164,166,814,802,811,169,161,160,
	162,171,215,211,168,170,810,808,981,980,895,212,214,894,892,893,983,979,988,989,887,883,880,882,891,218,890,888,842,879,876,878,866,864,867,877,841,840,843,
	881,889,884,886,846,844,847,885,991,975,1013,863,860,834,832,833,838,871,868,870,865,873,276,277,278,958,944,946,955,952,281,261,260,263,259,282,280,956,959,
	1045,1044,1046,1047,957,947,951,1053,1043,945,907,897,902,896,899,909,926,924,898,910,908,950,948,911,927,1077,1076,949,1055,1039,1052,1036,1054,1037,1027,1042,
	1040,1041,1049,1048,1051,1031,1028,1029,1050,869,837,836,858,856,1061,1062,1060,1063,1030,1025,1024,1033,1069,1059,1056,1058,1057,1065,851,861,1015,1012,360,361
	,416,417,363,425,354,366,364,355,365,422,420,436,399,437,1034,1082,1071,415,1068,1070,854,852,855,1021,1011,1020,1023,853,1066,1064,405,383,380,370,
	382,406,404,1067,407,403,413,398,396,438,386,384,385,390,423,420,421,367,376,378,330,379,409,401,400,412,414,402,397,387,391,411,410,408,389,388};
	for(int i = 0; i < grass_count; i++)
	{
		_level0[grass[i]].type = 0; 
		_level0[grass[i]].color = Vector3f(0.5,0.694,0.01);
		_level0[grass[i]].height = 1.0;
		if (i%2==0) _level0[grass[i]].color*=1.1;
	}

	

	_level0[772].type = 8;
	_level0[772].doorid = 0;
	_level0[772].color = Vector3f(0.30,0.20,0.15);
	_level0[772].height = 1.1;
	_level0[772].blocking = true;


	_level0[1118].type = 8;
	_level0[1118].doorid = 4;
	_level0[1118].color = Vector3f(0.30,0.20,0.15);
	_level0[1118].height = 1.1;	
	_level0[1118].blocking = true;

	_level0[1113].type = 8;
	_level0[1113].doorid = 3;
	_level0[1113].color = Vector3f(0.30,0.20,0.15);
	_level0[1113].height = 1.1;	
	_level0[1113].blocking = true;

	_level0[930].type = 8;
	_level0[930].doorid = 2;
	_level0[930].color = Vector3f(0.30,0.20,0.15);
	_level0[930].height = 1.1;
	_level0[930].blocking = true;

	_level0[1002].type = 6;
	_level0[1002].item = 0;
	_level0[1002].color = Vector3f(0.5,0.694,0.01);
	_level0[1002].height = 1;

	_level0[512].type = 6;
	_level0[512].item = 2;
	_level0[512].color = Vector3f(0.5,0.694,0.01);
	_level0[512].height = 1;

	_level0[1111].type = 6;
	_level0[1111].item = 3;
	_level0[1111].color =Vector3f(0.5,0.694,0.01);
	_level0[1111].height = 1;

	_level0[299].type = 6;
	_level0[299].item = 4;
	_level0[299].color = Vector3f(0.5,0.694,0.01);
	_level0[299].height = 1;

	

	const int enemy_count = 15;
	int enemy[] = {0,976,783,784,582,1097,1123,1121,1208,202,213,283,954,273,953};
	for(int i = 0; i < enemy_count; i++)
	{
		_level0[enemy[i]].type = 10; 
		_level0[enemy[i]].color = Vector3f(0.5,0.694,0.01);
		_level0[enemy[i]].height = 1;
	}

	
	const int water_count = 6+51+44;
	int water[] = {963,960,973,829,828,831,149,1237,1236,1239,1238,745,
		744,746,731,707,711,734,717,722,720,716,732,580,613,581,614,556,558,559,483,493,449,454,487,
		457,450,448,451,462,460,461,1032,1080,1081,1035,1026,1073,1074,1072,1073,1078,1038,1083,642,651,648,
	203,192,194,204,206,205,195,199,210,222,209,219,208,216,217,199,198,196,197,231,193,228,229,230,225,224,227,232,233,872,234,874,875,226,235,835,839,859,862,845,848,850,849,857};
	for(int i = 0; i < water_count; i++)
	{
		_level0[water[i]].type = 1; 
		_level0[water[i]].color = Vector3f(0,0.2,1);
		_level0[water[i]].height = 0.98;
		_level0[water[i]].blocking = true;
		if (water[i]%2==0) _level0[water[i]].color*=1.1;
	}
	
	const int pit_count = 10;
	int pit[] = {669,659,658,656,668,670,653,643,647,667};
	for(int i = 0; i < pit_count; i++)
	{
		_level0[pit[i]].type = 11; 
		_level0[pit[i]].color = Vector3f(0.3,0.1,0.1);
		_level0[pit[i]].height = 0.88;
		_level0[pit[i]].blocking = true;
	}
	*/


}

void World::tick(float time)
{
	
	_attack.tick(time);
	int attackTriangle=-1;
	const Vector3f attackP=_attack.pos();
	for (int i=0;i<currentLevel().triangleGraph().size();i++)
	{
		if (currentLevel().triangleGraph()[i].isInside(attackP))attackTriangle=i;
	}
	/*if (attackTriangle>=0)
	{
		//if (attackTriangle)
		if (currentLevel()[attackTriangle].type==10)
		{
			switch(_currentLevel)
			{
				case 0:_level0[attackTriangle].type=0; break;
				case 1:_level1[attackTriangle].type=0; break;
				case 2:_level2[attackTriangle].type=0; break;
			}	
		}
	}*/
	
	const int tri=_genie.triangle();
	if (tri>= 0 && currentLevel()[tri].type==Tile::portal)
	{
		_currentLevel=(_currentLevel+1)%maxLevel;
		_player->setTriangle(currentLevel().startTile());
		//_sfxEngine->playLevelSound();
	}
	
/*	if (tri>= 0 && currentLevel()[tri].type==10)
	{
		_currentLevel=0;
		_player->setTriangle(currentLevel().startTile());
		//_sfxEngine->playDeathSound();
	}
	if (tri>= 0 && currentLevel()[tri].type==6)
	{
		//_currentLevel=0;
		//_player->setTriangle(currentLevel().startTile());
		//_sfxEngine->playKeySound();
		int key=currentLevel()[tri].item;
		
		if (key>=0)
		{
		 	for (int i=0;i<currentLevel().size();i++)
			{
				if (currentLevel()[i].doorid==key)
				switch(_currentLevel)
				{
					case 0:_level0[i].blocking=false; break;
					case 1:_level1[i].blocking=false; break;
					case 2:_level2[i].blocking=false; break;
				}	
			}
			switch(_currentLevel)
			{
				case 0:_level0[tri].type=0;_level0[tri].color=Vector3f(0.5,0.694,0.01); break;
				case 1:_level1[tri].type=0;_level1[tri].color=Vector3f(0.5,0.694,0.01); break;
				case 2:_level2[tri].type=0;_level2[tri].color=Vector3f(0.5,0.694,0.01); break;
			}
		}
	}*/
	
	//_attack.print()	;
}
/*const World::Level& _level(int i)
{
		
}	
**/


