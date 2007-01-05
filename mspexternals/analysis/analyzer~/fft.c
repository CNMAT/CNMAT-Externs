/*
 * Copyright (c) 1992 Regents of the University of California.
 * All rights reserved.
 * The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * ffft.c
 * real and complex forward and inverse fft's
 */
         
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "fft.h"

#define INLINEBITREV // Use bitrev512
#define STORED // Use maketwiddle
// #define TFAST // Used in realfft
// #define GOOD 

#ifdef INLINEBITREV
/*
 *  Copyright 1990  Richard H. Krukar all rights reserved
 *  Permission granted to buy, sell, or steal this software is granted.
 *  The author retains the right to distribute this software freely, but
 *  is not responsible for it's quality or maintainance.
 */

/* bitrev512 */
static void bitrev512( float *x, int length ) {
	float temp;
	    
	switch (length) {
		case 512:
				temp= x[2]; x[2] = x[512]; x[512] = temp;
				temp= x[3]; x[3] = x[513]; x[513] = temp;
                temp= x[4]; x[4] = x[256]; x[256] = temp;
                temp= x[5]; x[5] = x[257]; x[257] = temp;
                temp= x[6]; x[6] = x[768]; x[768] = temp;
                temp= x[7]; x[7] = x[769]; x[769] = temp;
                temp= x[8]; x[8] = x[128]; x[128] = temp;
                temp= x[9]; x[9] = x[129]; x[129] = temp;
                temp= x[10]; x[10] = x[640]; x[640] = temp;
                temp= x[11]; x[11] = x[641]; x[641] = temp;
                temp= x[12]; x[12] = x[384]; x[384] = temp;
                temp= x[13]; x[13] = x[385]; x[385] = temp;
                temp= x[14]; x[14] = x[896]; x[896] = temp;
                temp= x[15]; x[15] = x[897]; x[897] = temp;
                temp= x[16]; x[16] = x[64]; x[64] = temp;
                temp= x[17]; x[17] = x[65]; x[65] = temp;
                temp= x[18]; x[18] = x[576]; x[576] = temp;
                temp= x[19]; x[19] = x[577]; x[577] = temp;
                temp= x[20]; x[20] = x[320]; x[320] = temp;
                temp= x[21]; x[21] = x[321]; x[321] = temp;
                temp= x[22]; x[22] = x[832]; x[832] = temp;
                temp= x[23]; x[23] = x[833]; x[833] = temp;
                temp= x[24]; x[24] = x[192]; x[192] = temp;
                temp= x[25]; x[25] = x[193]; x[193] = temp;
                temp= x[26]; x[26] = x[704]; x[704] = temp;
                temp= x[27]; x[27] = x[705]; x[705] = temp;
                temp= x[28]; x[28] = x[448]; x[448] = temp;
                temp= x[29]; x[29] = x[449]; x[449] = temp;
                temp= x[30]; x[30] = x[960]; x[960] = temp;
                temp= x[31]; x[31] = x[961]; x[961] = temp;
                temp= x[34]; x[34] = x[544]; x[544] = temp;
                temp= x[35]; x[35] = x[545]; x[545] = temp;
                temp= x[36]; x[36] = x[288]; x[288] = temp;
                temp= x[37]; x[37] = x[289]; x[289] = temp;
                temp= x[38]; x[38] = x[800]; x[800] = temp;
                temp= x[39]; x[39] = x[801]; x[801] = temp;
                temp= x[40]; x[40] = x[160]; x[160] = temp;
                temp= x[41]; x[41] = x[161]; x[161] = temp;
                temp= x[42]; x[42] = x[672]; x[672] = temp;
                temp= x[43]; x[43] = x[673]; x[673] = temp;
                temp= x[44]; x[44] = x[416]; x[416] = temp;
                temp= x[45]; x[45] = x[417]; x[417] = temp;
                temp= x[46]; x[46] = x[928]; x[928] = temp;
                temp= x[47]; x[47] = x[929]; x[929] = temp;
                temp= x[48]; x[48] = x[96]; x[96] = temp;
                temp= x[49]; x[49] = x[97]; x[97] = temp;
                temp= x[50]; x[50] = x[608]; x[608] = temp;
                temp= x[51]; x[51] = x[609]; x[609] = temp;
                temp= x[52]; x[52] = x[352]; x[352] = temp;
                temp= x[53]; x[53] = x[353]; x[353] = temp;
                temp= x[54]; x[54] = x[864]; x[864] = temp;
                temp= x[55]; x[55] = x[865]; x[865] = temp;
                temp= x[56]; x[56] = x[224]; x[224] = temp;
                temp= x[57]; x[57] = x[225]; x[225] = temp;
                temp= x[58]; x[58] = x[736]; x[736] = temp;
                temp= x[59]; x[59] = x[737]; x[737] = temp;
                temp= x[60]; x[60] = x[480]; x[480] = temp;
                temp= x[61]; x[61] = x[481]; x[481] = temp;
                temp= x[62]; x[62] = x[992]; x[992] = temp;
                temp= x[63]; x[63] = x[993]; x[993] = temp;
                temp= x[66]; x[66] = x[528]; x[528] = temp;
                temp= x[67]; x[67] = x[529]; x[529] = temp;
                temp= x[68]; x[68] = x[272]; x[272] = temp;
                temp= x[69]; x[69] = x[273]; x[273] = temp;
                temp= x[70]; x[70] = x[784]; x[784] = temp;
                temp= x[71]; x[71] = x[785]; x[785] = temp;
                temp= x[72]; x[72] = x[144]; x[144] = temp;
                temp= x[73]; x[73] = x[145]; x[145] = temp;
                temp= x[74]; x[74] = x[656]; x[656] = temp;
                temp= x[75]; x[75] = x[657]; x[657] = temp;
                temp= x[76]; x[76] = x[400]; x[400] = temp;
                temp= x[77]; x[77] = x[401]; x[401] = temp;
                temp= x[78]; x[78] = x[912]; x[912] = temp;
                temp= x[79]; x[79] = x[913]; x[913] = temp;
                temp= x[82]; x[82] = x[592]; x[592] = temp;
                temp= x[83]; x[83] = x[593]; x[593] = temp;
                temp= x[84]; x[84] = x[336]; x[336] = temp;
                temp= x[85]; x[85] = x[337]; x[337] = temp;
                temp= x[86]; x[86] = x[848]; x[848] = temp;
                temp= x[87]; x[87] = x[849]; x[849] = temp;
                temp= x[88]; x[88] = x[208]; x[208] = temp;
                temp= x[89]; x[89] = x[209]; x[209] = temp;
                temp= x[90]; x[90] = x[720]; x[720] = temp;
                temp= x[91]; x[91] = x[721]; x[721] = temp;
                temp= x[92]; x[92] = x[464]; x[464] = temp;
                temp= x[93]; x[93] = x[465]; x[465] = temp;
                temp= x[94]; x[94] = x[976]; x[976] = temp;
                temp= x[95]; x[95] = x[977]; x[977] = temp;
                temp= x[98]; x[98] = x[560]; x[560] = temp;
                temp= x[99]; x[99] = x[561]; x[561] = temp;
                temp= x[100]; x[100] = x[304]; x[304] = temp;
                temp= x[101]; x[101] = x[305]; x[305] = temp;
                temp= x[102]; x[102] = x[816]; x[816] = temp;
                temp= x[103]; x[103] = x[817]; x[817] = temp;
                temp= x[104]; x[104] = x[176]; x[176] = temp;
                temp= x[105]; x[105] = x[177]; x[177] = temp;
                temp= x[106]; x[106] = x[688]; x[688] = temp;
                temp= x[107]; x[107] = x[689]; x[689] = temp;
                temp= x[108]; x[108] = x[432]; x[432] = temp;
                temp= x[109]; x[109] = x[433]; x[433] = temp;
                temp= x[110]; x[110] = x[944]; x[944] = temp;
                temp= x[111]; x[111] = x[945]; x[945] = temp;
                temp= x[114]; x[114] = x[624]; x[624] = temp;
                temp= x[115]; x[115] = x[625]; x[625] = temp;
                temp= x[116]; x[116] = x[368]; x[368] = temp;
                temp= x[117]; x[117] = x[369]; x[369] = temp;
                temp= x[118]; x[118] = x[880]; x[880] = temp;
                temp= x[119]; x[119] = x[881]; x[881] = temp;
                temp= x[120]; x[120] = x[240]; x[240] = temp;
                temp= x[121]; x[121] = x[241]; x[241] = temp;
                temp= x[122]; x[122] = x[752]; x[752] = temp;
                temp= x[123]; x[123] = x[753]; x[753] = temp;
                temp= x[124]; x[124] = x[496]; x[496] = temp;
                temp= x[125]; x[125] = x[497]; x[497] = temp;
                temp= x[126]; x[126] = x[1008]; x[1008] = temp;
                temp= x[127]; x[127] = x[1009]; x[1009] = temp;
                temp= x[130]; x[130] = x[520]; x[520] = temp;
                temp= x[131]; x[131] = x[521]; x[521] = temp;
                temp= x[132]; x[132] = x[264]; x[264] = temp;
                temp= x[133]; x[133] = x[265]; x[265] = temp;
                temp= x[134]; x[134] = x[776]; x[776] = temp;
                temp= x[135]; x[135] = x[777]; x[777] = temp;
                temp= x[138]; x[138] = x[648]; x[648] = temp;
                temp= x[139]; x[139] = x[649]; x[649] = temp;
                temp= x[140]; x[140] = x[392]; x[392] = temp;
                temp= x[141]; x[141] = x[393]; x[393] = temp;
                temp= x[142]; x[142] = x[904]; x[904] = temp;
                temp= x[143]; x[143] = x[905]; x[905] = temp;
                temp= x[146]; x[146] = x[584]; x[584] = temp;
                temp= x[147]; x[147] = x[585]; x[585] = temp;
                temp= x[148]; x[148] = x[328]; x[328] = temp;
                temp= x[149]; x[149] = x[329]; x[329] = temp;
                temp= x[150]; x[150] = x[840]; x[840] = temp;
                temp= x[151]; x[151] = x[841]; x[841] = temp;
                temp= x[152]; x[152] = x[200]; x[200] = temp;
                temp= x[153]; x[153] = x[201]; x[201] = temp;
                temp= x[154]; x[154] = x[712]; x[712] = temp;
                temp= x[155]; x[155] = x[713]; x[713] = temp;
                temp= x[156]; x[156] = x[456]; x[456] = temp;
                temp= x[157]; x[157] = x[457]; x[457] = temp;
                temp= x[158]; x[158] = x[968]; x[968] = temp;
                temp= x[159]; x[159] = x[969]; x[969] = temp;
                temp= x[162]; x[162] = x[552]; x[552] = temp;
                temp= x[163]; x[163] = x[553]; x[553] = temp;
                temp= x[164]; x[164] = x[296]; x[296] = temp;
                temp= x[165]; x[165] = x[297]; x[297] = temp;
                temp= x[166]; x[166] = x[808]; x[808] = temp;
                temp= x[167]; x[167] = x[809]; x[809] = temp;
                temp= x[170]; x[170] = x[680]; x[680] = temp;
                temp= x[171]; x[171] = x[681]; x[681] = temp;
                temp= x[172]; x[172] = x[424]; x[424] = temp;
                temp= x[173]; x[173] = x[425]; x[425] = temp;
                temp= x[174]; x[174] = x[936]; x[936] = temp;
                temp= x[175]; x[175] = x[937]; x[937] = temp;
                temp= x[178]; x[178] = x[616]; x[616] = temp;
                temp= x[179]; x[179] = x[617]; x[617] = temp;
                temp= x[180]; x[180] = x[360]; x[360] = temp;
                temp= x[181]; x[181] = x[361]; x[361] = temp;
                temp= x[182]; x[182] = x[872]; x[872] = temp;
                temp= x[183]; x[183] = x[873]; x[873] = temp;
                temp= x[184]; x[184] = x[232]; x[232] = temp;
                temp= x[185]; x[185] = x[233]; x[233] = temp;
                temp= x[186]; x[186] = x[744]; x[744] = temp;
                temp= x[187]; x[187] = x[745]; x[745] = temp;
                temp= x[188]; x[188] = x[488]; x[488] = temp;
                temp= x[189]; x[189] = x[489]; x[489] = temp;
                temp= x[190]; x[190] = x[1000]; x[1000] = temp;
                temp= x[191]; x[191] = x[1001]; x[1001] = temp;
                temp= x[194]; x[194] = x[536]; x[536] = temp;
                temp= x[195]; x[195] = x[537]; x[537] = temp;
                temp= x[196]; x[196] = x[280]; x[280] = temp;
                temp= x[197]; x[197] = x[281]; x[281] = temp;
                temp= x[198]; x[198] = x[792]; x[792] = temp;
                temp= x[199]; x[199] = x[793]; x[793] = temp;
                temp= x[202]; x[202] = x[664]; x[664] = temp;
                temp= x[203]; x[203] = x[665]; x[665] = temp;
                temp= x[204]; x[204] = x[408]; x[408] = temp;
                temp= x[205]; x[205] = x[409]; x[409] = temp;
                temp= x[206]; x[206] = x[920]; x[920] = temp;
                temp= x[207]; x[207] = x[921]; x[921] = temp;
                temp= x[210]; x[210] = x[600]; x[600] = temp;
                temp= x[211]; x[211] = x[601]; x[601] = temp;
                temp= x[212]; x[212] = x[344]; x[344] = temp;
                temp= x[213]; x[213] = x[345]; x[345] = temp;
                temp= x[214]; x[214] = x[856]; x[856] = temp;
                temp= x[215]; x[215] = x[857]; x[857] = temp;
                temp= x[218]; x[218] = x[728]; x[728] = temp;
                temp= x[219]; x[219] = x[729]; x[729] = temp;
                temp= x[220]; x[220] = x[472]; x[472] = temp;
                temp= x[221]; x[221] = x[473]; x[473] = temp;
                temp= x[222]; x[222] = x[984]; x[984] = temp;
                temp= x[223]; x[223] = x[985]; x[985] = temp;
                temp= x[226]; x[226] = x[568]; x[568] = temp;
                temp= x[227]; x[227] = x[569]; x[569] = temp;
                temp= x[228]; x[228] = x[312]; x[312] = temp;
                temp= x[229]; x[229] = x[313]; x[313] = temp;
                temp= x[230]; x[230] = x[824]; x[824] = temp;
                temp= x[231]; x[231] = x[825]; x[825] = temp;
                temp= x[234]; x[234] = x[696]; x[696] = temp;
                temp= x[235]; x[235] = x[697]; x[697] = temp;
                temp= x[236]; x[236] = x[440]; x[440] = temp;
                temp= x[237]; x[237] = x[441]; x[441] = temp;
                temp= x[238]; x[238] = x[952]; x[952] = temp;
                temp= x[239]; x[239] = x[953]; x[953] = temp;
                temp= x[242]; x[242] = x[632]; x[632] = temp;
                temp= x[243]; x[243] = x[633]; x[633] = temp;
                temp= x[244]; x[244] = x[376]; x[376] = temp;
                temp= x[245]; x[245] = x[377]; x[377] = temp;
                temp= x[246]; x[246] = x[888]; x[888] = temp;
                temp= x[247]; x[247] = x[889]; x[889] = temp;
                temp= x[250]; x[250] = x[760]; x[760] = temp;
                temp= x[251]; x[251] = x[761]; x[761] = temp;
                temp= x[252]; x[252] = x[504]; x[504] = temp;
                temp= x[253]; x[253] = x[505]; x[505] = temp;
                temp= x[254]; x[254] = x[1016]; x[1016] = temp;
                temp= x[255]; x[255] = x[1017]; x[1017] = temp;
                temp= x[258]; x[258] = x[516]; x[516] = temp;
                temp= x[259]; x[259] = x[517]; x[517] = temp;
                temp= x[262]; x[262] = x[772]; x[772] = temp;
                temp= x[263]; x[263] = x[773]; x[773] = temp;
                temp= x[266]; x[266] = x[644]; x[644] = temp;
                temp= x[267]; x[267] = x[645]; x[645] = temp;
                temp= x[268]; x[268] = x[388]; x[388] = temp;
                temp= x[269]; x[269] = x[389]; x[389] = temp;
                temp= x[270]; x[270] = x[900]; x[900] = temp;
                temp= x[271]; x[271] = x[901]; x[901] = temp;
                temp= x[274]; x[274] = x[580]; x[580] = temp;
                temp= x[275]; x[275] = x[581]; x[581] = temp;
                temp= x[276]; x[276] = x[324]; x[324] = temp;
                temp= x[277]; x[277] = x[325]; x[325] = temp;
                temp= x[278]; x[278] = x[836]; x[836] = temp;
                temp= x[279]; x[279] = x[837]; x[837] = temp;
                temp= x[282]; x[282] = x[708]; x[708] = temp;
                temp= x[283]; x[283] = x[709]; x[709] = temp;
                temp= x[284]; x[284] = x[452]; x[452] = temp;
                temp= x[285]; x[285] = x[453]; x[453] = temp;
                temp= x[286]; x[286] = x[964]; x[964] = temp;
                temp= x[287]; x[287] = x[965]; x[965] = temp;
                temp= x[290]; x[290] = x[548]; x[548] = temp;
                temp= x[291]; x[291] = x[549]; x[549] = temp;
                temp= x[294]; x[294] = x[804]; x[804] = temp;
                temp= x[295]; x[295] = x[805]; x[805] = temp;
                temp= x[298]; x[298] = x[676]; x[676] = temp;
                temp= x[299]; x[299] = x[677]; x[677] = temp;
                temp= x[300]; x[300] = x[420]; x[420] = temp;
                temp= x[301]; x[301] = x[421]; x[421] = temp;
                temp= x[302]; x[302] = x[932]; x[932] = temp;
                temp= x[303]; x[303] = x[933]; x[933] = temp;
                temp= x[306]; x[306] = x[612]; x[612] = temp;
                temp= x[307]; x[307] = x[613]; x[613] = temp;
                temp= x[308]; x[308] = x[356]; x[356] = temp;
                temp= x[309]; x[309] = x[357]; x[357] = temp;
                temp= x[310]; x[310] = x[868]; x[868] = temp;
                temp= x[311]; x[311] = x[869]; x[869] = temp;
                temp= x[314]; x[314] = x[740]; x[740] = temp;
                temp= x[315]; x[315] = x[741]; x[741] = temp;
                temp= x[316]; x[316] = x[484]; x[484] = temp;
                temp= x[317]; x[317] = x[485]; x[485] = temp;
                temp= x[318]; x[318] = x[996]; x[996] = temp;
                temp= x[319]; x[319] = x[997]; x[997] = temp;
                temp= x[322]; x[322] = x[532]; x[532] = temp;
                temp= x[323]; x[323] = x[533]; x[533] = temp;
                temp= x[326]; x[326] = x[788]; x[788] = temp;
                temp= x[327]; x[327] = x[789]; x[789] = temp;
                temp= x[330]; x[330] = x[660]; x[660] = temp;
                temp= x[331]; x[331] = x[661]; x[661] = temp;
                temp= x[332]; x[332] = x[404]; x[404] = temp;
                temp= x[333]; x[333] = x[405]; x[405] = temp;
                temp= x[334]; x[334] = x[916]; x[916] = temp;
                temp= x[335]; x[335] = x[917]; x[917] = temp;
                temp= x[338]; x[338] = x[596]; x[596] = temp;
                temp= x[339]; x[339] = x[597]; x[597] = temp;
                temp= x[342]; x[342] = x[852]; x[852] = temp;
                temp= x[343]; x[343] = x[853]; x[853] = temp;
                temp= x[346]; x[346] = x[724]; x[724] = temp;
                temp= x[347]; x[347] = x[725]; x[725] = temp;
                temp= x[348]; x[348] = x[468]; x[468] = temp;
                temp= x[349]; x[349] = x[469]; x[469] = temp;
                temp= x[350]; x[350] = x[980]; x[980] = temp;
                temp= x[351]; x[351] = x[981]; x[981] = temp;
                temp= x[354]; x[354] = x[564]; x[564] = temp;
                temp= x[355]; x[355] = x[565]; x[565] = temp;
                temp= x[358]; x[358] = x[820]; x[820] = temp;
                temp= x[359]; x[359] = x[821]; x[821] = temp;
                temp= x[362]; x[362] = x[692]; x[692] = temp;
                temp= x[363]; x[363] = x[693]; x[693] = temp;
                temp= x[364]; x[364] = x[436]; x[436] = temp;
                temp= x[365]; x[365] = x[437]; x[437] = temp;
                temp= x[366]; x[366] = x[948]; x[948] = temp;
                temp= x[367]; x[367] = x[949]; x[949] = temp;
                temp= x[370]; x[370] = x[628]; x[628] = temp;
                temp= x[371]; x[371] = x[629]; x[629] = temp;
                temp= x[374]; x[374] = x[884]; x[884] = temp;
                temp= x[375]; x[375] = x[885]; x[885] = temp;
                temp= x[378]; x[378] = x[756]; x[756] = temp;
                temp= x[379]; x[379] = x[757]; x[757] = temp;
                temp= x[380]; x[380] = x[500]; x[500] = temp;
                temp= x[381]; x[381] = x[501]; x[501] = temp;
                temp= x[382]; x[382] = x[1012]; x[1012] = temp;
                temp= x[383]; x[383] = x[1013]; x[1013] = temp;
                temp= x[386]; x[386] = x[524]; x[524] = temp;
                temp= x[387]; x[387] = x[525]; x[525] = temp;
                temp= x[390]; x[390] = x[780]; x[780] = temp;
                temp= x[391]; x[391] = x[781]; x[781] = temp;
                temp= x[394]; x[394] = x[652]; x[652] = temp;
                temp= x[395]; x[395] = x[653]; x[653] = temp;
                temp= x[398]; x[398] = x[908]; x[908] = temp;
                temp= x[399]; x[399] = x[909]; x[909] = temp;
                temp= x[402]; x[402] = x[588]; x[588] = temp;
                temp= x[403]; x[403] = x[589]; x[589] = temp;
                temp= x[406]; x[406] = x[844]; x[844] = temp;
                temp= x[407]; x[407] = x[845]; x[845] = temp;
                temp= x[410]; x[410] = x[716]; x[716] = temp;
                temp= x[411]; x[411] = x[717]; x[717] = temp;
                temp= x[412]; x[412] = x[460]; x[460] = temp;
                temp= x[413]; x[413] = x[461]; x[461] = temp;
                temp= x[414]; x[414] = x[972]; x[972] = temp;
                temp= x[415]; x[415] = x[973]; x[973] = temp;
                temp= x[418]; x[418] = x[556]; x[556] = temp;
                temp= x[419]; x[419] = x[557]; x[557] = temp;
                temp= x[422]; x[422] = x[812]; x[812] = temp;
                temp= x[423]; x[423] = x[813]; x[813] = temp;
                temp= x[426]; x[426] = x[684]; x[684] = temp;
                temp= x[427]; x[427] = x[685]; x[685] = temp;
                temp= x[430]; x[430] = x[940]; x[940] = temp;
                temp= x[431]; x[431] = x[941]; x[941] = temp;
                temp= x[434]; x[434] = x[620]; x[620] = temp;
                temp= x[435]; x[435] = x[621]; x[621] = temp;
                temp= x[438]; x[438] = x[876]; x[876] = temp;
                temp= x[439]; x[439] = x[877]; x[877] = temp;
                temp= x[442]; x[442] = x[748]; x[748] = temp;
                temp= x[443]; x[443] = x[749]; x[749] = temp;
                temp= x[444]; x[444] = x[492]; x[492] = temp;
                temp= x[445]; x[445] = x[493]; x[493] = temp;
                temp= x[446]; x[446] = x[1004]; x[1004] = temp;
                temp= x[447]; x[447] = x[1005]; x[1005] = temp;
                temp= x[450]; x[450] = x[540]; x[540] = temp;
                temp= x[451]; x[451] = x[541]; x[541] = temp;
                temp= x[454]; x[454] = x[796]; x[796] = temp;
                temp= x[455]; x[455] = x[797]; x[797] = temp;
                temp= x[458]; x[458] = x[668]; x[668] = temp;
                temp= x[459]; x[459] = x[669]; x[669] = temp;
                temp= x[462]; x[462] = x[924]; x[924] = temp;
                temp= x[463]; x[463] = x[925]; x[925] = temp;
                temp= x[466]; x[466] = x[604]; x[604] = temp;
                temp= x[467]; x[467] = x[605]; x[605] = temp;
                temp= x[470]; x[470] = x[860]; x[860] = temp;
                temp= x[471]; x[471] = x[861]; x[861] = temp;
                temp= x[474]; x[474] = x[732]; x[732] = temp;
                temp= x[475]; x[475] = x[733]; x[733] = temp;
                temp= x[478]; x[478] = x[988]; x[988] = temp;
                temp= x[479]; x[479] = x[989]; x[989] = temp;
                temp= x[482]; x[482] = x[572]; x[572] = temp;
                temp= x[483]; x[483] = x[573]; x[573] = temp;
                temp= x[486]; x[486] = x[828]; x[828] = temp;
                temp= x[487]; x[487] = x[829]; x[829] = temp;
                temp= x[490]; x[490] = x[700]; x[700] = temp;
                temp= x[491]; x[491] = x[701]; x[701] = temp;
                temp= x[494]; x[494] = x[956]; x[956] = temp;
                temp= x[495]; x[495] = x[957]; x[957] = temp;
                temp= x[498]; x[498] = x[636]; x[636] = temp;
                temp= x[499]; x[499] = x[637]; x[637] = temp;
                temp= x[502]; x[502] = x[892]; x[892] = temp;
                temp= x[503]; x[503] = x[893]; x[893] = temp;
                temp= x[506]; x[506] = x[764]; x[764] = temp;
                temp= x[507]; x[507] = x[765]; x[765] = temp;
                temp= x[510]; x[510] = x[1020]; x[1020] = temp;
                temp= x[511]; x[511] = x[1021]; x[1021] = temp;
                temp= x[518]; x[518] = x[770]; x[770] = temp;
                temp= x[519]; x[519] = x[771]; x[771] = temp;
                temp= x[522]; x[522] = x[642]; x[642] = temp;
                temp= x[523]; x[523] = x[643]; x[643] = temp;
                temp= x[526]; x[526] = x[898]; x[898] = temp;
                temp= x[527]; x[527] = x[899]; x[899] = temp;
                temp= x[530]; x[530] = x[578]; x[578] = temp;
                temp= x[531]; x[531] = x[579]; x[579] = temp;
                temp= x[534]; x[534] = x[834]; x[834] = temp;
                temp= x[535]; x[535] = x[835]; x[835] = temp;
                temp= x[538]; x[538] = x[706]; x[706] = temp;
                temp= x[539]; x[539] = x[707]; x[707] = temp;
                temp= x[542]; x[542] = x[962]; x[962] = temp;
                temp= x[543]; x[543] = x[963]; x[963] = temp;
                temp= x[550]; x[550] = x[802]; x[802] = temp;
                temp= x[551]; x[551] = x[803]; x[803] = temp;
                temp= x[554]; x[554] = x[674]; x[674] = temp;
                temp= x[555]; x[555] = x[675]; x[675] = temp;
                temp= x[558]; x[558] = x[930]; x[930] = temp;
                temp= x[559]; x[559] = x[931]; x[931] = temp;
                temp= x[562]; x[562] = x[610]; x[610] = temp;
                temp= x[563]; x[563] = x[611]; x[611] = temp;
                temp= x[566]; x[566] = x[866]; x[866] = temp;
                temp= x[567]; x[567] = x[867]; x[867] = temp;
                temp= x[570]; x[570] = x[738]; x[738] = temp;
                temp= x[571]; x[571] = x[739]; x[739] = temp;
                temp= x[574]; x[574] = x[994]; x[994] = temp;
                temp= x[575]; x[575] = x[995]; x[995] = temp;
                temp= x[582]; x[582] = x[786]; x[786] = temp;
                temp= x[583]; x[583] = x[787]; x[787] = temp;
                temp= x[586]; x[586] = x[658]; x[658] = temp;
                temp= x[587]; x[587] = x[659]; x[659] = temp;
                temp= x[590]; x[590] = x[914]; x[914] = temp;
                temp= x[591]; x[591] = x[915]; x[915] = temp;
                temp= x[598]; x[598] = x[850]; x[850] = temp;
                temp= x[599]; x[599] = x[851]; x[851] = temp;
                temp= x[602]; x[602] = x[722]; x[722] = temp;
                temp= x[603]; x[603] = x[723]; x[723] = temp;
                temp= x[606]; x[606] = x[978]; x[978] = temp;
                temp= x[607]; x[607] = x[979]; x[979] = temp;
                temp= x[614]; x[614] = x[818]; x[818] = temp;
                temp= x[615]; x[615] = x[819]; x[819] = temp;
                temp= x[618]; x[618] = x[690]; x[690] = temp;
                temp= x[619]; x[619] = x[691]; x[691] = temp;
                temp= x[622]; x[622] = x[946]; x[946] = temp;
                temp= x[623]; x[623] = x[947]; x[947] = temp;
                temp= x[630]; x[630] = x[882]; x[882] = temp;
                temp= x[631]; x[631] = x[883]; x[883] = temp;
                temp= x[634]; x[634] = x[754]; x[754] = temp;
                temp= x[635]; x[635] = x[755]; x[755] = temp;
                temp= x[638]; x[638] = x[1010]; x[1010] = temp;
                temp= x[639]; x[639] = x[1011]; x[1011] = temp;
                temp= x[646]; x[646] = x[778]; x[778] = temp;
                temp= x[647]; x[647] = x[779]; x[779] = temp;
                temp= x[654]; x[654] = x[906]; x[906] = temp;
                temp= x[655]; x[655] = x[907]; x[907] = temp;
                temp= x[662]; x[662] = x[842]; x[842] = temp;
                temp= x[663]; x[663] = x[843]; x[843] = temp;
                temp= x[666]; x[666] = x[714]; x[714] = temp;
                temp= x[667]; x[667] = x[715]; x[715] = temp;
                temp= x[670]; x[670] = x[970]; x[970] = temp;
                temp= x[671]; x[671] = x[971]; x[971] = temp;
                temp= x[678]; x[678] = x[810]; x[810] = temp;
                temp= x[679]; x[679] = x[811]; x[811] = temp;
                temp= x[686]; x[686] = x[938]; x[938] = temp;
                temp= x[687]; x[687] = x[939]; x[939] = temp;
                temp= x[694]; x[694] = x[874]; x[874] = temp;
                temp= x[695]; x[695] = x[875]; x[875] = temp;
                temp= x[698]; x[698] = x[746]; x[746] = temp;
                temp= x[699]; x[699] = x[747]; x[747] = temp;
                temp= x[702]; x[702] = x[1002]; x[1002] = temp;
                temp= x[703]; x[703] = x[1003]; x[1003] = temp;
                temp= x[710]; x[710] = x[794]; x[794] = temp;
                temp= x[711]; x[711] = x[795]; x[795] = temp;
                temp= x[718]; x[718] = x[922]; x[922] = temp;
                temp= x[719]; x[719] = x[923]; x[923] = temp;
                temp= x[726]; x[726] = x[858]; x[858] = temp;
                temp= x[727]; x[727] = x[859]; x[859] = temp;
                temp= x[734]; x[734] = x[986]; x[986] = temp;
                temp= x[735]; x[735] = x[987]; x[987] = temp;
                temp= x[742]; x[742] = x[826]; x[826] = temp;
                temp= x[743]; x[743] = x[827]; x[827] = temp;
                temp= x[750]; x[750] = x[954]; x[954] = temp;
                temp= x[751]; x[751] = x[955]; x[955] = temp;
                temp= x[758]; x[758] = x[890]; x[890] = temp;
                temp= x[759]; x[759] = x[891]; x[891] = temp;
                temp= x[766]; x[766] = x[1018]; x[1018] = temp;
                temp= x[767]; x[767] = x[1019]; x[1019] = temp;
                temp= x[782]; x[782] = x[902]; x[902] = temp;
                temp= x[783]; x[783] = x[903]; x[903] = temp;
                temp= x[790]; x[790] = x[838]; x[838] = temp;
                temp= x[791]; x[791] = x[839]; x[839] = temp;
                temp= x[798]; x[798] = x[966]; x[966] = temp;
                temp= x[799]; x[799] = x[967]; x[967] = temp;
                temp= x[814]; x[814] = x[934]; x[934] = temp;
                temp= x[815]; x[815] = x[935]; x[935] = temp;
                temp= x[822]; x[822] = x[870]; x[870] = temp;
                temp= x[823]; x[823] = x[871]; x[871] = temp;
                temp= x[830]; x[830] = x[998]; x[998] = temp;
                temp= x[831]; x[831] = x[999]; x[999] = temp;
                temp= x[846]; x[846] = x[918]; x[918] = temp;
                temp= x[847]; x[847] = x[919]; x[919] = temp;
                temp= x[862]; x[862] = x[982]; x[982] = temp;
                temp= x[863]; x[863] = x[983]; x[983] = temp;
                temp= x[878]; x[878] = x[950]; x[950] = temp;
                temp= x[879]; x[879] = x[951]; x[951] = temp;
                temp= x[894]; x[894] = x[1014]; x[1014] = temp;
                temp= x[895]; x[895] = x[1015]; x[1015] = temp;
                temp= x[926]; x[926] = x[974]; x[974] = temp;
                temp= x[927]; x[927] = x[975]; x[975] = temp;
                temp= x[958]; x[958] = x[1006]; x[1006] = temp;
                temp= x[959]; x[959] = x[1007]; x[1007] = temp;
                break;
		case 256:
                temp= x[2]; x[2] = x[256]; x[256] = temp;
                temp= x[3]; x[3] = x[257]; x[257] = temp;
                temp= x[4]; x[4] = x[128]; x[128] = temp;
                temp= x[5]; x[5] = x[129]; x[129] = temp;
                temp= x[6]; x[6] = x[384]; x[384] = temp;
                temp= x[7]; x[7] = x[385]; x[385] = temp;
                temp= x[8]; x[8] = x[64]; x[64] = temp;
                temp= x[9]; x[9] = x[65]; x[65] = temp;
                temp= x[10]; x[10] = x[320]; x[320] = temp;
                temp= x[11]; x[11] = x[321]; x[321] = temp;
                temp= x[12]; x[12] = x[192]; x[192] = temp;
                temp= x[13]; x[13] = x[193]; x[193] = temp;
                temp= x[14]; x[14] = x[448]; x[448] = temp;
                temp= x[15]; x[15] = x[449]; x[449] = temp;
                temp= x[16]; x[16] = x[32]; x[32] = temp;
                temp= x[17]; x[17] = x[33]; x[33] = temp;
                temp= x[18]; x[18] = x[288]; x[288] = temp;
                temp= x[19]; x[19] = x[289]; x[289] = temp;
                temp= x[20]; x[20] = x[160]; x[160] = temp;
                temp= x[21]; x[21] = x[161]; x[161] = temp;
                temp= x[22]; x[22] = x[416]; x[416] = temp;
                temp= x[23]; x[23] = x[417]; x[417] = temp;
                temp= x[24]; x[24] = x[96]; x[96] = temp;
                temp= x[25]; x[25] = x[97]; x[97] = temp;
                temp= x[26]; x[26] = x[352]; x[352] = temp;
                temp= x[27]; x[27] = x[353]; x[353] = temp;
                temp= x[28]; x[28] = x[224]; x[224] = temp;
                temp= x[29]; x[29] = x[225]; x[225] = temp;
                temp= x[30]; x[30] = x[480]; x[480] = temp;
                temp= x[31]; x[31] = x[481]; x[481] = temp;
                temp= x[34]; x[34] = x[272]; x[272] = temp;
                temp= x[35]; x[35] = x[273]; x[273] = temp;
                temp= x[36]; x[36] = x[144]; x[144] = temp;
                temp= x[37]; x[37] = x[145]; x[145] = temp;
                temp= x[38]; x[38] = x[400]; x[400] = temp;
                temp= x[39]; x[39] = x[401]; x[401] = temp;
                temp= x[40]; x[40] = x[80]; x[80] = temp;
                temp= x[41]; x[41] = x[81]; x[81] = temp;
                temp= x[42]; x[42] = x[336]; x[336] = temp;
                temp= x[43]; x[43] = x[337]; x[337] = temp;
                temp= x[44]; x[44] = x[208]; x[208] = temp;
                temp= x[45]; x[45] = x[209]; x[209] = temp;
                temp= x[46]; x[46] = x[464]; x[464] = temp;
                temp= x[47]; x[47] = x[465]; x[465] = temp;
                temp= x[50]; x[50] = x[304]; x[304] = temp;
                temp= x[51]; x[51] = x[305]; x[305] = temp;
                temp= x[52]; x[52] = x[176]; x[176] = temp;
                temp= x[53]; x[53] = x[177]; x[177] = temp;
                temp= x[54]; x[54] = x[432]; x[432] = temp;
                temp= x[55]; x[55] = x[433]; x[433] = temp;
                temp= x[56]; x[56] = x[112]; x[112] = temp;
                temp= x[57]; x[57] = x[113]; x[113] = temp;
                temp= x[58]; x[58] = x[368]; x[368] = temp;
                temp= x[59]; x[59] = x[369]; x[369] = temp;
                temp= x[60]; x[60] = x[240]; x[240] = temp;
                temp= x[61]; x[61] = x[241]; x[241] = temp;
                temp= x[62]; x[62] = x[496]; x[496] = temp;
                temp= x[63]; x[63] = x[497]; x[497] = temp;
                temp= x[66]; x[66] = x[264]; x[264] = temp;
                temp= x[67]; x[67] = x[265]; x[265] = temp;
                temp= x[68]; x[68] = x[136]; x[136] = temp;
                temp= x[69]; x[69] = x[137]; x[137] = temp;
                temp= x[70]; x[70] = x[392]; x[392] = temp;
                temp= x[71]; x[71] = x[393]; x[393] = temp;
                temp= x[74]; x[74] = x[328]; x[328] = temp;
                temp= x[75]; x[75] = x[329]; x[329] = temp;
                temp= x[76]; x[76] = x[200]; x[200] = temp;
                temp= x[77]; x[77] = x[201]; x[201] = temp;
                temp= x[78]; x[78] = x[456]; x[456] = temp;
                temp= x[79]; x[79] = x[457]; x[457] = temp;
                temp= x[82]; x[82] = x[296]; x[296] = temp;
                temp= x[83]; x[83] = x[297]; x[297] = temp;
                temp= x[84]; x[84] = x[168]; x[168] = temp;
                temp= x[85]; x[85] = x[169]; x[169] = temp;
                temp= x[86]; x[86] = x[424]; x[424] = temp;
                temp= x[87]; x[87] = x[425]; x[425] = temp;
                temp= x[88]; x[88] = x[104]; x[104] = temp;
                temp= x[89]; x[89] = x[105]; x[105] = temp;
                temp= x[90]; x[90] = x[360]; x[360] = temp;
                temp= x[91]; x[91] = x[361]; x[361] = temp;
                temp= x[92]; x[92] = x[232]; x[232] = temp;
                temp= x[93]; x[93] = x[233]; x[233] = temp;
                temp= x[94]; x[94] = x[488]; x[488] = temp;
                temp= x[95]; x[95] = x[489]; x[489] = temp;
                temp= x[98]; x[98] = x[280]; x[280] = temp;
                temp= x[99]; x[99] = x[281]; x[281] = temp;
                temp= x[100]; x[100] = x[152]; x[152] = temp;
                temp= x[101]; x[101] = x[153]; x[153] = temp;
                temp= x[102]; x[102] = x[408]; x[408] = temp;
                temp= x[103]; x[103] = x[409]; x[409] = temp;
                temp= x[106]; x[106] = x[344]; x[344] = temp;
                temp= x[107]; x[107] = x[345]; x[345] = temp;
                temp= x[108]; x[108] = x[216]; x[216] = temp;
                temp= x[109]; x[109] = x[217]; x[217] = temp;
                temp= x[110]; x[110] = x[472]; x[472] = temp;
                temp= x[111]; x[111] = x[473]; x[473] = temp;
                temp= x[114]; x[114] = x[312]; x[312] = temp;
                temp= x[115]; x[115] = x[313]; x[313] = temp;
                temp= x[116]; x[116] = x[184]; x[184] = temp;
                temp= x[117]; x[117] = x[185]; x[185] = temp;
                temp= x[118]; x[118] = x[440]; x[440] = temp;
                temp= x[119]; x[119] = x[441]; x[441] = temp;
                temp= x[122]; x[122] = x[376]; x[376] = temp;
                temp= x[123]; x[123] = x[377]; x[377] = temp;
                temp= x[124]; x[124] = x[248]; x[248] = temp;
                temp= x[125]; x[125] = x[249]; x[249] = temp;
                temp= x[126]; x[126] = x[504]; x[504] = temp;
                temp= x[127]; x[127] = x[505]; x[505] = temp;
                temp= x[130]; x[130] = x[260]; x[260] = temp;
                temp= x[131]; x[131] = x[261]; x[261] = temp;
                temp= x[134]; x[134] = x[388]; x[388] = temp;
                temp= x[135]; x[135] = x[389]; x[389] = temp;
                temp= x[138]; x[138] = x[324]; x[324] = temp;
                temp= x[139]; x[139] = x[325]; x[325] = temp;
                temp= x[140]; x[140] = x[196]; x[196] = temp;
                temp= x[141]; x[141] = x[197]; x[197] = temp;
                temp= x[142]; x[142] = x[452]; x[452] = temp;
                temp= x[143]; x[143] = x[453]; x[453] = temp;
                temp= x[146]; x[146] = x[292]; x[292] = temp;
                temp= x[147]; x[147] = x[293]; x[293] = temp;
                temp= x[148]; x[148] = x[164]; x[164] = temp;
                temp= x[149]; x[149] = x[165]; x[165] = temp;
                temp= x[150]; x[150] = x[420]; x[420] = temp;
                temp= x[151]; x[151] = x[421]; x[421] = temp;
                temp= x[154]; x[154] = x[356]; x[356] = temp;
                temp= x[155]; x[155] = x[357]; x[357] = temp;
                temp= x[156]; x[156] = x[228]; x[228] = temp;
                temp= x[157]; x[157] = x[229]; x[229] = temp;
                temp= x[158]; x[158] = x[484]; x[484] = temp;
                temp= x[159]; x[159] = x[485]; x[485] = temp;
                temp= x[162]; x[162] = x[276]; x[276] = temp;
                temp= x[163]; x[163] = x[277]; x[277] = temp;
                temp= x[166]; x[166] = x[404]; x[404] = temp;
                temp= x[167]; x[167] = x[405]; x[405] = temp;
                temp= x[170]; x[170] = x[340]; x[340] = temp;
                temp= x[171]; x[171] = x[341]; x[341] = temp;
                temp= x[172]; x[172] = x[212]; x[212] = temp;
                temp= x[173]; x[173] = x[213]; x[213] = temp;
                temp= x[174]; x[174] = x[468]; x[468] = temp;
                temp= x[175]; x[175] = x[469]; x[469] = temp;
                temp= x[178]; x[178] = x[308]; x[308] = temp;
                temp= x[179]; x[179] = x[309]; x[309] = temp;
                temp= x[182]; x[182] = x[436]; x[436] = temp;
                temp= x[183]; x[183] = x[437]; x[437] = temp;
                temp= x[186]; x[186] = x[372]; x[372] = temp;
                temp= x[187]; x[187] = x[373]; x[373] = temp;
                temp= x[188]; x[188] = x[244]; x[244] = temp;
                temp= x[189]; x[189] = x[245]; x[245] = temp;
                temp= x[190]; x[190] = x[500]; x[500] = temp;
                temp= x[191]; x[191] = x[501]; x[501] = temp;
                temp= x[194]; x[194] = x[268]; x[268] = temp;
                temp= x[195]; x[195] = x[269]; x[269] = temp;
                temp= x[198]; x[198] = x[396]; x[396] = temp;
                temp= x[199]; x[199] = x[397]; x[397] = temp;
                temp= x[202]; x[202] = x[332]; x[332] = temp;
                temp= x[203]; x[203] = x[333]; x[333] = temp;
                temp= x[206]; x[206] = x[460]; x[460] = temp;
                temp= x[207]; x[207] = x[461]; x[461] = temp;
                temp= x[210]; x[210] = x[300]; x[300] = temp;
                temp= x[211]; x[211] = x[301]; x[301] = temp;
                temp= x[214]; x[214] = x[428]; x[428] = temp;
                temp= x[215]; x[215] = x[429]; x[429] = temp;
                temp= x[218]; x[218] = x[364]; x[364] = temp;
                temp= x[219]; x[219] = x[365]; x[365] = temp;
                temp= x[220]; x[220] = x[236]; x[236] = temp;
                temp= x[221]; x[221] = x[237]; x[237] = temp;
                temp= x[222]; x[222] = x[492]; x[492] = temp;
                temp= x[223]; x[223] = x[493]; x[493] = temp;
                temp= x[226]; x[226] = x[284]; x[284] = temp;
                temp= x[227]; x[227] = x[285]; x[285] = temp;
                temp= x[230]; x[230] = x[412]; x[412] = temp;
                temp= x[231]; x[231] = x[413]; x[413] = temp;
                temp= x[234]; x[234] = x[348]; x[348] = temp;
                temp= x[235]; x[235] = x[349]; x[349] = temp;
                temp= x[238]; x[238] = x[476]; x[476] = temp;
                temp= x[239]; x[239] = x[477]; x[477] = temp;
                temp= x[242]; x[242] = x[316]; x[316] = temp;
                temp= x[243]; x[243] = x[317]; x[317] = temp;
                temp= x[246]; x[246] = x[444]; x[444] = temp;
                temp= x[247]; x[247] = x[445]; x[445] = temp;
                temp= x[250]; x[250] = x[380]; x[380] = temp;
                temp= x[251]; x[251] = x[381]; x[381] = temp;
                temp= x[254]; x[254] = x[508]; x[508] = temp;
                temp= x[255]; x[255] = x[509]; x[509] = temp;
                temp= x[262]; x[262] = x[386]; x[386] = temp;
                temp= x[263]; x[263] = x[387]; x[387] = temp;
                temp= x[266]; x[266] = x[322]; x[322] = temp;
                temp= x[267]; x[267] = x[323]; x[323] = temp;
                temp= x[270]; x[270] = x[450]; x[450] = temp;
                temp= x[271]; x[271] = x[451]; x[451] = temp;
                temp= x[274]; x[274] = x[290]; x[290] = temp;
                temp= x[275]; x[275] = x[291]; x[291] = temp;
                temp= x[278]; x[278] = x[418]; x[418] = temp;
                temp= x[279]; x[279] = x[419]; x[419] = temp;
                temp= x[282]; x[282] = x[354]; x[354] = temp;
                temp= x[283]; x[283] = x[355]; x[355] = temp;
                temp= x[286]; x[286] = x[482]; x[482] = temp;
                temp= x[287]; x[287] = x[483]; x[483] = temp;
                temp= x[294]; x[294] = x[402]; x[402] = temp;
                temp= x[295]; x[295] = x[403]; x[403] = temp;
                temp= x[298]; x[298] = x[338]; x[338] = temp;
                temp= x[299]; x[299] = x[339]; x[339] = temp;
                temp= x[302]; x[302] = x[466]; x[466] = temp;
                temp= x[303]; x[303] = x[467]; x[467] = temp;
                temp= x[310]; x[310] = x[434]; x[434] = temp;
                temp= x[311]; x[311] = x[435]; x[435] = temp;
                temp= x[314]; x[314] = x[370]; x[370] = temp;
                temp= x[315]; x[315] = x[371]; x[371] = temp;
                temp= x[318]; x[318] = x[498]; x[498] = temp;
                temp= x[319]; x[319] = x[499]; x[499] = temp;
                temp= x[326]; x[326] = x[394]; x[394] = temp;
                temp= x[327]; x[327] = x[395]; x[395] = temp;
                temp= x[334]; x[334] = x[458]; x[458] = temp;
                temp= x[335]; x[335] = x[459]; x[459] = temp;
                temp= x[342]; x[342] = x[426]; x[426] = temp;
                temp= x[343]; x[343] = x[427]; x[427] = temp;
                temp= x[346]; x[346] = x[362]; x[362] = temp;
                temp= x[347]; x[347] = x[363]; x[363] = temp;
                temp= x[350]; x[350] = x[490]; x[490] = temp;
                temp= x[351]; x[351] = x[491]; x[491] = temp;
                temp= x[358]; x[358] = x[410]; x[410] = temp;
                temp= x[359]; x[359] = x[411]; x[411] = temp;
                temp= x[366]; x[366] = x[474]; x[474] = temp;
                temp= x[367]; x[367] = x[475]; x[475] = temp;
                temp= x[374]; x[374] = x[442]; x[442] = temp;
                temp= x[375]; x[375] = x[443]; x[443] = temp;
                temp= x[382]; x[382] = x[506]; x[506] = temp;
                temp= x[383]; x[383] = x[507]; x[507] = temp;
                temp= x[398]; x[398] = x[454]; x[454] = temp;
                temp= x[399]; x[399] = x[455]; x[455] = temp;
                temp= x[406]; x[406] = x[422]; x[422] = temp;
                temp= x[407]; x[407] = x[423]; x[423] = temp;
                temp= x[414]; x[414] = x[486]; x[486] = temp;
                temp= x[415]; x[415] = x[487]; x[487] = temp;
                temp= x[430]; x[430] = x[470]; x[470] = temp;
                temp= x[431]; x[431] = x[471]; x[471] = temp;
                temp= x[446]; x[446] = x[502]; x[502] = temp;
                temp= x[447]; x[447] = x[503]; x[503] = temp;
                temp= x[478]; x[478] = x[494]; x[494] = temp;
                temp= x[479]; x[479] = x[495]; x[495] = temp;
                break;
		case 128:
                temp= x[2]; x[2] = x[128]; x[128] = temp;
                temp= x[3]; x[3] = x[129]; x[129] = temp;
                temp= x[4]; x[4] = x[64]; x[64] = temp;
                temp= x[5]; x[5] = x[65]; x[65] = temp;
                temp= x[6]; x[6] = x[192]; x[192] = temp;
                temp= x[7]; x[7] = x[193]; x[193] = temp;
                temp= x[8]; x[8] = x[32]; x[32] = temp;
                temp= x[9]; x[9] = x[33]; x[33] = temp;
                temp= x[10]; x[10] = x[160]; x[160] = temp;
                temp= x[11]; x[11] = x[161]; x[161] = temp;
                temp= x[12]; x[12] = x[96]; x[96] = temp;
                temp= x[13]; x[13] = x[97]; x[97] = temp;
                temp= x[14]; x[14] = x[224]; x[224] = temp;
                temp= x[15]; x[15] = x[225]; x[225] = temp;
                temp= x[18]; x[18] = x[144]; x[144] = temp;
                temp= x[19]; x[19] = x[145]; x[145] = temp;
                temp= x[20]; x[20] = x[80]; x[80] = temp;
                temp= x[21]; x[21] = x[81]; x[81] = temp;
                temp= x[22]; x[22] = x[208]; x[208] = temp;
                temp= x[23]; x[23] = x[209]; x[209] = temp;
                temp= x[24]; x[24] = x[48]; x[48] = temp;
                temp= x[25]; x[25] = x[49]; x[49] = temp;
                temp= x[26]; x[26] = x[176]; x[176] = temp;
                temp= x[27]; x[27] = x[177]; x[177] = temp;
                temp= x[28]; x[28] = x[112]; x[112] = temp;
                temp= x[29]; x[29] = x[113]; x[113] = temp;
                temp= x[30]; x[30] = x[240]; x[240] = temp;
                temp= x[31]; x[31] = x[241]; x[241] = temp;
                temp= x[34]; x[34] = x[136]; x[136] = temp;
                temp= x[35]; x[35] = x[137]; x[137] = temp;
                temp= x[36]; x[36] = x[72]; x[72] = temp;
                temp= x[37]; x[37] = x[73]; x[73] = temp;
                temp= x[38]; x[38] = x[200]; x[200] = temp;
                temp= x[39]; x[39] = x[201]; x[201] = temp;
                temp= x[42]; x[42] = x[168]; x[168] = temp;
                temp= x[43]; x[43] = x[169]; x[169] = temp;
                temp= x[44]; x[44] = x[104]; x[104] = temp;
                temp= x[45]; x[45] = x[105]; x[105] = temp;
                temp= x[46]; x[46] = x[232]; x[232] = temp;
                temp= x[47]; x[47] = x[233]; x[233] = temp;
                temp= x[50]; x[50] = x[152]; x[152] = temp;
                temp= x[51]; x[51] = x[153]; x[153] = temp;
                temp= x[52]; x[52] = x[88]; x[88] = temp;
                temp= x[53]; x[53] = x[89]; x[89] = temp;
                temp= x[54]; x[54] = x[216]; x[216] = temp;
                temp= x[55]; x[55] = x[217]; x[217] = temp;
                temp= x[58]; x[58] = x[184]; x[184] = temp;
                temp= x[59]; x[59] = x[185]; x[185] = temp;
                temp= x[60]; x[60] = x[120]; x[120] = temp;
                temp= x[61]; x[61] = x[121]; x[121] = temp;
                temp= x[62]; x[62] = x[248]; x[248] = temp;
                temp= x[63]; x[63] = x[249]; x[249] = temp;
                temp= x[66]; x[66] = x[132]; x[132] = temp;
                temp= x[67]; x[67] = x[133]; x[133] = temp;
                temp= x[70]; x[70] = x[196]; x[196] = temp;
                temp= x[71]; x[71] = x[197]; x[197] = temp;
                temp= x[74]; x[74] = x[164]; x[164] = temp;
                temp= x[75]; x[75] = x[165]; x[165] = temp;
                temp= x[76]; x[76] = x[100]; x[100] = temp;
                temp= x[77]; x[77] = x[101]; x[101] = temp;
                temp= x[78]; x[78] = x[228]; x[228] = temp;
                temp= x[79]; x[79] = x[229]; x[229] = temp;
                temp= x[82]; x[82] = x[148]; x[148] = temp;
                temp= x[83]; x[83] = x[149]; x[149] = temp;
                temp= x[86]; x[86] = x[212]; x[212] = temp;
                temp= x[87]; x[87] = x[213]; x[213] = temp;
                temp= x[90]; x[90] = x[180]; x[180] = temp;
                temp= x[91]; x[91] = x[181]; x[181] = temp;
                temp= x[92]; x[92] = x[116]; x[116] = temp;
                temp= x[93]; x[93] = x[117]; x[117] = temp;
                temp= x[94]; x[94] = x[244]; x[244] = temp;
                temp= x[95]; x[95] = x[245]; x[245] = temp;
                temp= x[98]; x[98] = x[140]; x[140] = temp;
                temp= x[99]; x[99] = x[141]; x[141] = temp;
                temp= x[102]; x[102] = x[204]; x[204] = temp;
                temp= x[103]; x[103] = x[205]; x[205] = temp;
                temp= x[106]; x[106] = x[172]; x[172] = temp;
                temp= x[107]; x[107] = x[173]; x[173] = temp;
                temp= x[110]; x[110] = x[236]; x[236] = temp;
                temp= x[111]; x[111] = x[237]; x[237] = temp;
                temp= x[114]; x[114] = x[156]; x[156] = temp;
                temp= x[115]; x[115] = x[157]; x[157] = temp;
                temp= x[118]; x[118] = x[220]; x[220] = temp;
                temp= x[119]; x[119] = x[221]; x[221] = temp;
                temp= x[122]; x[122] = x[188]; x[188] = temp;
                temp= x[123]; x[123] = x[189]; x[189] = temp;
                temp= x[126]; x[126] = x[252]; x[252] = temp;
                temp= x[127]; x[127] = x[253]; x[253] = temp;
                temp= x[134]; x[134] = x[194]; x[194] = temp;
                temp= x[135]; x[135] = x[195]; x[195] = temp;
                temp= x[138]; x[138] = x[162]; x[162] = temp;
                temp= x[139]; x[139] = x[163]; x[163] = temp;
                temp= x[142]; x[142] = x[226]; x[226] = temp;
                temp= x[143]; x[143] = x[227]; x[227] = temp;
                temp= x[150]; x[150] = x[210]; x[210] = temp;
                temp= x[151]; x[151] = x[211]; x[211] = temp;
                temp= x[154]; x[154] = x[178]; x[178] = temp;
                temp= x[155]; x[155] = x[179]; x[179] = temp;
                temp= x[158]; x[158] = x[242]; x[242] = temp;
                temp= x[159]; x[159] = x[243]; x[243] = temp;
                temp= x[166]; x[166] = x[202]; x[202] = temp;
                temp= x[167]; x[167] = x[203]; x[203] = temp;
                temp= x[174]; x[174] = x[234]; x[234] = temp;
                temp= x[175]; x[175] = x[235]; x[235] = temp;
                temp= x[182]; x[182] = x[218]; x[218] = temp;
                temp= x[183]; x[183] = x[219]; x[219] = temp;
                temp= x[190]; x[190] = x[250]; x[250] = temp;
                temp= x[191]; x[191] = x[251]; x[251] = temp;
                temp= x[206]; x[206] = x[230]; x[230] = temp;
                temp= x[207]; x[207] = x[231]; x[231] = temp;
                temp= x[222]; x[222] = x[246]; x[246] = temp;
                temp= x[223]; x[223] = x[247]; x[247] = temp;
                break;
		case 64:
                temp= x[2]; x[2] = x[64]; x[64] = temp;
                temp= x[3]; x[3] = x[65]; x[65] = temp;
                temp= x[4]; x[4] = x[32]; x[32] = temp;
                temp= x[5]; x[5] = x[33]; x[33] = temp;
                temp= x[6]; x[6] = x[96]; x[96] = temp;
                temp= x[7]; x[7] = x[97]; x[97] = temp;
                temp= x[8]; x[8] = x[16]; x[16] = temp;
                temp= x[9]; x[9] = x[17]; x[17] = temp;
                temp= x[10]; x[10] = x[80]; x[80] = temp;
                temp= x[11]; x[11] = x[81]; x[81] = temp;
                temp= x[12]; x[12] = x[48]; x[48] = temp;
                temp= x[13]; x[13] = x[49]; x[49] = temp;
                temp= x[14]; x[14] = x[112]; x[112] = temp;
                temp= x[15]; x[15] = x[113]; x[113] = temp;
                temp= x[18]; x[18] = x[72]; x[72] = temp;
                temp= x[19]; x[19] = x[73]; x[73] = temp;
                temp= x[20]; x[20] = x[40]; x[40] = temp;
                temp= x[21]; x[21] = x[41]; x[41] = temp;
                temp= x[22]; x[22] = x[104]; x[104] = temp;
                temp= x[23]; x[23] = x[105]; x[105] = temp;
                temp= x[26]; x[26] = x[88]; x[88] = temp;
                temp= x[27]; x[27] = x[89]; x[89] = temp;
                temp= x[28]; x[28] = x[56]; x[56] = temp;
                temp= x[29]; x[29] = x[57]; x[57] = temp;
                temp= x[30]; x[30] = x[120]; x[120] = temp;
                temp= x[31]; x[31] = x[121]; x[121] = temp;
                temp= x[34]; x[34] = x[68]; x[68] = temp;
                temp= x[35]; x[35] = x[69]; x[69] = temp;
                temp= x[38]; x[38] = x[100]; x[100] = temp;
                temp= x[39]; x[39] = x[101]; x[101] = temp;
                temp= x[42]; x[42] = x[84]; x[84] = temp;
                temp= x[43]; x[43] = x[85]; x[85] = temp;
                temp= x[44]; x[44] = x[52]; x[52] = temp;
                temp= x[45]; x[45] = x[53]; x[53] = temp;
                temp= x[46]; x[46] = x[116]; x[116] = temp;
                temp= x[47]; x[47] = x[117]; x[117] = temp;
                temp= x[50]; x[50] = x[76]; x[76] = temp;
                temp= x[51]; x[51] = x[77]; x[77] = temp;
                temp= x[54]; x[54] = x[108]; x[108] = temp;
                temp= x[55]; x[55] = x[109]; x[109] = temp;
                temp= x[58]; x[58] = x[92]; x[92] = temp;
                temp= x[59]; x[59] = x[93]; x[93] = temp;
                temp= x[62]; x[62] = x[124]; x[124] = temp;
                temp= x[63]; x[63] = x[125]; x[125] = temp;
                temp= x[70]; x[70] = x[98]; x[98] = temp;
                temp= x[71]; x[71] = x[99]; x[99] = temp;
                temp= x[74]; x[74] = x[82]; x[82] = temp;
                temp= x[75]; x[75] = x[83]; x[83] = temp;
                temp= x[78]; x[78] = x[114]; x[114] = temp;
                temp= x[79]; x[79] = x[115]; x[115] = temp;
                temp= x[86]; x[86] = x[106]; x[106] = temp;
                temp= x[87]; x[87] = x[107]; x[107] = temp;
                temp= x[94]; x[94] = x[122]; x[122] = temp;
                temp= x[95]; x[95] = x[123]; x[123] = temp;
                temp= x[110]; x[110] = x[118]; x[118] = temp;
                temp= x[111]; x[111] = x[119]; x[119] = temp;
                break;
		case 32:
                temp= x[2]; x[2] = x[32]; x[32] = temp;
                temp= x[3]; x[3] = x[33]; x[33] = temp;
                temp= x[4]; x[4] = x[16]; x[16] = temp;
                temp= x[5]; x[5] = x[17]; x[17] = temp;
                temp= x[6]; x[6] = x[48]; x[48] = temp;
                temp= x[7]; x[7] = x[49]; x[49] = temp;
                temp= x[10]; x[10] = x[40]; x[40] = temp;
                temp= x[11]; x[11] = x[41]; x[41] = temp;
                temp= x[12]; x[12] = x[24]; x[24] = temp;
                temp= x[13]; x[13] = x[25]; x[25] = temp;
                temp= x[14]; x[14] = x[56]; x[56] = temp;
                temp= x[15]; x[15] = x[57]; x[57] = temp;
                temp= x[18]; x[18] = x[36]; x[36] = temp;
                temp= x[19]; x[19] = x[37]; x[37] = temp;
                temp= x[22]; x[22] = x[52]; x[52] = temp;
                temp= x[23]; x[23] = x[53]; x[53] = temp;
                temp= x[26]; x[26] = x[44]; x[44] = temp;
                temp= x[27]; x[27] = x[45]; x[45] = temp;
                temp= x[30]; x[30] = x[60]; x[60] = temp;
                temp= x[31]; x[31] = x[61]; x[61] = temp;
                temp= x[38]; x[38] = x[50]; x[50] = temp;
                temp= x[39]; x[39] = x[51]; x[51] = temp;
                temp= x[46]; x[46] = x[58]; x[58] = temp;
                temp= x[47]; x[47] = x[59]; x[59] = temp;
                break;
		case 16:
                temp= x[2]; x[2] = x[16]; x[16] = temp;
                temp= x[3]; x[3] = x[17]; x[17] = temp;
                temp= x[4]; x[4] = x[8]; x[8] = temp;
                temp= x[5]; x[5] = x[9]; x[9] = temp;
                temp= x[6]; x[6] = x[24]; x[24] = temp;
                temp= x[7]; x[7] = x[25]; x[25] = temp;
                temp= x[10]; x[10] = x[20]; x[20] = temp;
                temp= x[11]; x[11] = x[21]; x[21] = temp;
                temp= x[14]; x[14] = x[28]; x[28] = temp;
                temp= x[15]; x[15] = x[29]; x[29] = temp;
                temp= x[22]; x[22] = x[26]; x[26] = temp;
                temp= x[23]; x[23] = x[27]; x[27] = temp;
                break;
		case 8:
                temp= x[2]; x[2] = x[8]; x[8] = temp;
                temp= x[3]; x[3] = x[9]; x[9] = temp;
                temp= x[6]; x[6] = x[12]; x[12] = temp;
                temp= x[7]; x[7] = x[13]; x[13] = temp;
                break;
		case 4:
                temp= x[2]; x[2] = x[4]; x[4] = temp;
                temp= x[3]; x[3] = x[5]; x[5] = temp;
                break;
		case 2:
                break;
	}
}
#endif

static float *twiddle = 0;
static int ncount; 
static struct {
		short ncache;
        float *twiddle;
        Boolean ffttype;
} c[CMAX];

/* maketwiddle */
static void maketwiddle(int n, Boolean notinverse, float* PtrMemory) {
	int i;
        
	/* cache twiddle factors */
	for (i=0; i<CMAX; ++i) {
		if (n==c[i].ncache && c[i].ffttype==notinverse) {
			twiddle = c[i].twiddle;
			return;
		}
	}
	{
		double pi = PI;
		float *tp;
	
		c[ncount].ncache = n;
		c[ncount].ffttype = notinverse;
		c[ncount].twiddle = &PtrMemory[ncount*n];
		twiddle = c[ncount].twiddle;

		if(!twiddle) 
			return;

		ncount = (ncount+1) % CMAX;
		tp = twiddle;

		/* compute twiddle factors */
		{
			double arg;
			double cos_arg, sin_arg;
                         
			if(notinverse)
				arg = 2.0*PI/n;
			else
				arg = -2.0*PI/n;
                                        
			*tp++ = 1.0f;
			*tp++ = 0.0f;
			cos_arg = cos(arg);
			sin_arg = sin(arg);
			*tp++ = cos_arg;
			*tp++ = sin_arg;
			tp -= 2;
			for (i=2; i<n-1; i++) {
				*(tp+2) = *tp *cos_arg - *(tp+1)*sin_arg;
				*(tp+3) = *tp *sin_arg + *(tp+1)*cos_arg;
				tp += 2;
			}
		}
	}
}

/*
 * complex fft
 * real and complex components are stored alternately in *a
 * n is number of points
 * calculation is in place 
 */
  
void fftComplex(int n, float *a, Boolean notinverse, float* PtrMemory) {
	long i, j, mmax, m, N;
	float t, tr,tc;
    long istep;
        
    --a;
        
	/* bit reversal */
    N = n*2;
#ifdef INLINEBITREV
    if (n<=512)
		bitrev512(a+1,n);
    else
#endif
    {
		j=1;
		for (i=1; i<N; i+=2) {
			if(j>i) {
				t = a[j];
				a[j] = a[i];
				a[i] = t;
				t = a[i+1];
				a[i+1] = a[j+1];
				a[j+1] = t;
			}
			m = n;
			while (m>=2 && j>m) {
				j -= m;
				m >>= 1;
			}
			j += m;
		}
	}
#ifdef STORED
	maketwiddle(n,notinverse,PtrMemory);
#endif

	/* fft itself */
	{
		long uNQmq, NQmq;

#ifdef STORED
		float ur, uc;
#else
		double ur, uc, wpr, wpi, wpin, theta, wtemp;
#endif
		float ai,aiplus1;
		mmax = 2;
		NQmq = n;

#ifndef STORED
		theta = PI * 0.5;
		if(!notinverse)
			theta = - theta;
		wpin = 0.0;
#endif
                        
		while (N>mmax) {

#ifndef STORED
			wpi = wpin;
			wpin = sin(theta);
			wpr = 1.0 - wpin*wpin - wpin*wpin;
			theta *= 0.5;
			ur = 1.0;
			uc = 0.0;
#endif                
			istep = mmax<<1;

#ifdef GOOD
			for (j=1, uNQmq=0; j<mmax; j+=2, uNQmq+=NQmq) {
#ifdef STORED
 				ur = twiddle[uNQmq];
				uc = twiddle[uNQmq+1];
#endif
				for (i=j; i<=N; i+=istep) {
						int k = i+mmax;

						/* complex multiply of a by u */
						tr = (float)ur*a[k] - (float)uc*a[k+1];
						tc = (float)uc*a[k] + (float)ur*a[k+1];
						
						/* butterfly */
						a[k] = (ai=a[i]) - tr;
						a[i] = ai + tr;
						a[k+1] = (aiplus1=a[i+1]) - tc;
						a[i+1] = aiplus1+tc;
				}
#ifndef STORED
				ur = (wtemp = ur)*wpr - uc*wpi;
				uc = wtemp*wpi + uc*wpr;
#endif
			}                        
#else // no GOOD

			for (j=1, uNQmq=0; j<mmax; j+=2, uNQmq+=NQmq) {
				float d1r, d1c, *kkp, *kp, *ap;
#ifdef STORED
				ur = twiddle[uNQmq];
				uc = twiddle[uNQmq+1];
#endif
				tr = (float)ur*(d1r=a[j+mmax]);
				tc = (float)uc*(d1c=a[j+mmax+1]);

				for (i=j, kp=&a[mmax+j], ap=&a[j], kkp=&a[mmax+j+istep]; 
						i<=(N-istep); i+=istep, ap+=istep, kp+=istep, kkp+=istep) {

					/* complex multiply of a by u */
					tr -= tc;
					tc = (float)uc*d1r + (float)ur*d1c;

					/* butterfly */
					d1r = *kkp;
					d1c = kkp[1];
					*kp = (ai=*ap) - tr;
					*ap = ai + tr;
					kp[1] = (aiplus1=ap[1]) - tc;
					ap[1] = aiplus1+tc;
					tr = (float)ur*d1r; 
					tc = (float)uc*d1c;
				}
				{  
					int k = i+mmax;
					
					/* complex multiply of a by u */
					tr -= tc;
					tc = (float)uc*d1r + (float)ur*d1c;
					
					/* butterfly */
					a[k] = (ai=a[i]) - tr;
					a[i] = ai + tr;
					a[k+1] = (aiplus1=a[i+1]) - tc;
					a[i+1] = aiplus1+tc;
				}

#ifndef STORED
				ur = (wtemp = ur)*wpr - uc*wpi;
				uc = wtemp*wpi + uc*wpr;
#endif
			}
#endif

			mmax = istep;
	 		NQmq >>= 1;                     
		}
	}
	if (!notinverse) {
		float scale = 1.0f/n;
	
		for (i=1; i<=n*2; ++i)
			a[i] *= scale;
	}
}

/* 
 * realfft
 * for real valued fft's this scrambles and unscrambles to use
 * a complexfft (above) of half the size
 */ 
 
void realfft(int n, float *a, Boolean notinverse, float* PtrMemory) {
	int i, i1, i2, i3, i4, n2p3;
	float c1=0.5f, c2, h1r, h1i, h2r, h2i;

#ifdef TFAST
	float wr, wi;
#else
	double wr, wi, wpr, wpi, wtemp, theta;
	theta = PI/n;
#endif  

	if (notinverse) {
		c2= -0.5f;
		fftComplex(n,a,notinverse,PtrMemory);
	} else {
		c2=0.5f;
#ifndef TFAST
		theta = -theta;
#endif
	}
	--a;
	maketwiddle(2*n,notinverse,PtrMemory);

#ifndef TFAST
	wtemp = sin(0.5*theta);
	wpr = -2.0 *wtemp*wtemp;
	wpi = sin(theta);
	wr = 1.0+wpr;
	wi = wpi;
#endif  

	n2p3 = 2*n+3;
	for (i=2; i<=n/2; ++i) {
		i4 = 1+ (i3=n2p3-(i2=1+(i1=i+i-1)));
		h1r = c1*(a[i1] + a[i3]);
		h1i = c1*(a[i2] - a[i4]);
		h2r = -c2*(a[i2] + a[i4]);
		h2i = c2*(a[i1]-a[i3]);

#ifdef TFAST
		wr = twiddle[2*(i-1)];
		wi = twiddle[2*(i-1)+1];
#endif
		a[i1] = h1r+wr*h2r-wi*h2i;
		a[i2] = h1i+wr*h2i+wi*h2r;
		a[i3] = h1r-wr*h2r+wi*h2i;
		a[i4] = -h1i+wr*h2i+wi*h2r;

#ifndef TFAST
		wr = (wtemp=wr)*wpr -wi*wpi+wr;
		wi = wi*wpr+wtemp*wpi+wi;
//		printf("%f %f %f %f\n", twiddle[2*(i-1)], wr, twiddle[2*(i-1)+1], wi);
#endif
	}
		
	if (notinverse) {
		a[1] = (h1r=a[1])+a[2];
		a[2] = h1r-a[2];
	} else {
		a[1] = c1*((h1r=a[1])+a[2]);
		a[2] = c1*(h1r-a[2]);
		fftComplex(n, a+1, notinverse, PtrMemory);
	}        
}

/* fftRealfast */
void fftRealfast(int n, float *r, float* PtrMemory) {
	realfft(n/2, r, TRUE, PtrMemory);
}  

/* ifftRealfast */
void ifftRealfast(int n, float *rc, float* PtrMemory) {
	realfft( n/2, rc, FALSE, PtrMemory);
}
