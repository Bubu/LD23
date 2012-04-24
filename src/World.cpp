#include <World.h>
#include <iostream>
World::World():_level0(Level(3,0)),_level1(Level(3,1)),_level2(Level(3,2)),maxLevel(3),_currentLevel(0),_attack(Projectile(0.0f, 0.0f, Matrix3x3f()))
{
	_level0[997].color= Vector3f(1,0,0); //start tile

	const int end_count = 6;
	int end[] = {693,1119,1103,1141,671,655};
	for(int i = 0; i < end_count; i++)
	{
		_level0[end[i]].type = 5; 
		_level0[end[i]].color = Vector3f(1,0,1);
		_level0[end[i]].height = 1.01;

	}

	const int mountain_count = 21+4+21+22+28+21+9+24+56+71;
	int mountain[] = {792,793,998,996,999,804,806,985,807,795,774,768,769,786,798,803,801,800,982,809,977,974,962,972,1014,
	1007,995,349,1018,1009,1010,371,374,351,333,331,321,347,992,1000,340,638,635,325,352,353,
	586,590,578,621,615,628,631,1278,341,1276,791,589,594,601,523,571,566,513,518,551,619,616,618,426,490,
	424,427,362,554,430,418,428,393,419,429,392,395,441,433,435,432,439,1143,652,654,694,692,695,1117,1107,
	516,536,507,508,509,1150,1136,1138,1137,1145,1099,1090,1088,1091,537,529,535,1194,661,440,474,913,912,915,1075,1079,925,1193,1185,1192,
	1104,1105,1095,1114,1092,1093,933,935,931,928,1110,1108,1109,938,297,298,296,289,294,941,905,904,906,292,
	1008,1022,381,368,369,377,328,329,626,1275,1266,1277,588,629,591,606,593,592,514,526,525,524,542,540,543,1199,
	1196,1195,1190,1188,1158,1191,562,573,574,572,1162,1160,1163,1209,1201,1215,575,725,724,723,727,719,757,1180,1183,1182,1165,1159,1155,1156,
	703,700,687,650,684,686,674,39,6,1,21,20,23,19,16,18,28,30,31,15,53,52,55,51,49,48,59,50,56,58,10,47,95,36,37,679,676,678,1177,1181,
	1171,1175,765,32,33,38,40,43,41,87,84,81,86,755,753,752,761,762,760,79,76,78,90,69,68,70,67,64,66,64,65};
	for(int i = 0; i < mountain_count; i++)
	{
		_level0[mountain[i]].type = 1; 
		_level0[mountain[i]].color = Vector3f(0.447,0.38,0.286);
		_level0[mountain[i]].height = 1.1;
		_level0[mountain[i]].blocking = true;
		if (mountain[i]%2==0) _level0[mountain[i]].color*=1.1;
	}

	const int grass_count = 5+11+16+87+14+79+39+12+43+73;
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
	763,89,88,91,71,77,94,82,80,83,93,92,46,34,44,35,45,9,8,11,57,2,54,14,12,13,3,7,4,5,26,24,27,698,696,697,689,688,690,702,22,17,25,699,691,701};
	for(int i = 0; i < grass_count; i++)
	{
		_level0[grass[i]].type = 0; 
		_level0[grass[i]].color = Vector3f(0.5,0.694,0.01);
		_level0[grass[i]].height = 1.0;
		if (i%2==0) _level0[grass[i]].color*=1.1;
	}

	/*_level0[794].type = 7; 
	_level0[794].color = Vector3f(1,1,1);
	_level0[794].height = 1.0;*/

	_level0[772].type = 8;
	_level0[772].doorid = 0;
	_level0[772].color = Vector3f(0.30,0.20,0.15);
	_level0[772].height = 1.1;

	_level0[1118].type = 8;
	_level0[1118].doorid = 4;
	_level0[1118].color = Vector3f(0.30,0.20,0.15);
	_level0[1118].height = 1.1;

	_level0[1113].type = 8;
	_level0[1113].doorid = 3;
	_level0[1113].color = Vector3f(0.30,0.20,0.15);
	_level0[1113].height = 1.1;

	_level0[930].type = 8;
	_level0[930].doorid = 2;
	_level0[930].color = Vector3f(0.30,0.20,0.15);
	_level0[930].height = 1.1;

	_level0[1002].type = 6;
	_level0[1002].item = 0;
	_level0[1002].color = Vector3f(239,183,52) / 255;
	_level0[1002].height = 1;

	_level0[512].type = 6;
	_level0[512].item = 2;
	_level0[512].color = Vector3f(239,183,52) / 255;
	_level0[512].height = 1;

	_level0[1111].type = 6;
	_level0[1111].item = 3;
	_level0[1111].color = Vector3f(239,183,52) / 255;
	_level0[1111].height = 1;

	/*_level0[512].type = 6;
	_level0[512].item = 1;
	_level0[512].color = Vector3f(239,183,52) / 255;
	_level0[512].height = 1;*/

	const int enemy_count = 9;
	int enemy[] = {0,976,783,784,582,1097,1123,1121,1208};
	for(int i = 0; i < enemy_count; i++)
	{
		_level0[enemy[i]].type = 10; 
		_level0[enemy[i]].color = Vector3f(1,0,0);//Vector3f(0.5,0.694,0.01)
		_level0[enemy[i]].height = 1;
	}

	
	const int water_count = 6+51;
	int water[] = {963,960,973,829,828,831,149,1237,1236,1239,1238,745,
		744,746,731,707,711,734,717,722,720,716,732,580,613,581,614,556,558,559,483,493,449,454,487,
		457,450,448,451,462,460,461,1032,1080,1081,1035,1026,1073,1074,1072,1073,1078,1038,1083,642,651,648};
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
	


}

void World::tick(float time)
{
	
	_attack.tick(time);
	const int tri=_genie.triangle();
	if (tri>= 0 && currentLevel()[tri].type==5)
	{
		_currentLevel=(_currentLevel+1)%maxLevel;
		_player->setTriangle(currentLevel().startTile());
	}
	
	if (tri>= 0 && currentLevel()[tri].type==10)
	{
		_currentLevel=0;
		_player->setTriangle(currentLevel().startTile());
	}
	
	//_attack.print()	;
}
/*const World::Level& _level(int i)
{
		
}	
**/


