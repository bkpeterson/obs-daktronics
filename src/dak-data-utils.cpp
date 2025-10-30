#include "dak-data-utils.hpp"

void DAKDataUtils::read_csv_field(std::stringstream &ss, std::string &field)
{
	std::getline(ss, field, '\t');
}

void DAKDataUtils::read_csv_field(std::stringstream &ss, uint32_t &field)
{
	std::string temp;
	std::getline(ss, temp, '\t');
	field = std::stoi(temp);
}

uint32_t DAKDataUtils::readDataLine(std::stringstream &is, DAKTSVData &sportData)
{
	std::string line;
	if (std::getline(is, line)) {
		if (line.empty()) {
			//End of data category
			return 1;
		}

		std::stringstream iss(line);
		read_csv_field(iss, sportData.index);
		read_csv_field(iss, sportData.field);
		read_csv_field(iss, sportData.length);
	} else {
		//EOF
		return -1;
	}

	return 0;
}

uint32_t DAKDataUtils::readHeaderLine(std::stringstream &is, DAKTSVHeader &sportData)
{
	std::string line;
	if (std::getline(is, line)) {
		std::stringstream iss(line);
		read_csv_field(iss, sportData.sport);
		read_csv_field(iss, sportData.size);

		return 0;

	} else {
		//EOF
		return -1;
	}
}

void DAKDataUtils::populateSportsData()
{
	_DAKData[0] =
		"Basketball\t646\n1\tMain Clock Time (mm:ss, ss.t)\t5\n6\tMain Clock Time (mm:ss.t)\t8\n14\tMain Clock, Time Out Time, Time of Day (mm:ss ss.t)\t5\n19\tMain Clock, Time Out Time, Time of Day (mm:ss.t)\t8\n27\tMain Clock = 00:00\t1\n28\tMain Clock Stopped\t1\n29\tMain Clock or Time Out Horn\t1\n30\tMain Clock Horn\t1\n31\tTime Out Horn\t1\n32\tTime Out Time (mm:ss)\t8\n40\tTime of Day (hh:mm:ss)\t8\n48\tHome Team Name\t20\n68\tGuest Team Name\t20\n88\tHome Team Abbreviation\t10\n98\tGuest Team Abbreviation\t10\n108\tHome Team Score\t4\n112\tGuest Team Score\t4\n116\tHome Time Outs Left - Full\t2\n118\tHome Time Outs Left - Partial\t2\n120\tHome Time Outs Left - Television\t2\n122\tHome Time Outs Left - Total\t2\n124\tGuest Time Outs Left - Full\t2\n126\tGuest Time Outs Left - Partial\t2\n128\tGuest Time Outs Left - Television\t2\n130\tGuest Time Outs Left - Total\t2\n132\tHome Time Out Indicator\t1\n133\tHome Time Out Text\t4\n137\tGuest Time Out Indicator\t1\n138\tGuest Time Out Text\t4\n142\tPeriod\t2\n144\tPeriod Text\t4\n148\tPeriod Description\t12\n160\tInternal Relay\t1\n161\tAd Panel Caption Power\t1\n162\tAd Panel Caption #1\t1\n163\tAd Panel Caption #2\t1\n164\tAd Panel Caption #3\t1\n165\tAd Panel Caption #4\t1\n166\tReserved for Future Use\t35\n201\tShot Clock Time (mm:ss)\t8\n209\tShot Clock Horn\t1\n210\tHome Possession Indicator\t1\n211\tHome Possession Arrow\t1\n212\tHome Possession Text\t4\n216\tGuest Possession Indicator\t1\n217\tGuest Possession Arrow\t1\n218\tGuest Possession Text\t4\n222\tHome 1-on-1 Bonus Indicator\t1\n223\tHome 2-shot Bonus Indicator\t1\n224\tHome Bonus Text\t5\n229\tGuest 1-on-1 Bonus Indicator\t1\n230\tGuest 2-shot Bonus Indicator\t1\n231\tGuest Bonus Text\t5\n236\tHome Team Fouls\t2\n238\tGuest Team Fouls\t2\n240\tHome Player-Foul-Points\t8\n248\tGuest Player-Foul-Points\t8\n256\tPlayer-Foul\t3\n259\tPlayer-Foul-Points\t5\n264\tHome Score - Period 1\t2\n266\tHome Score - Period 2\t2\n268\tHome Score - Period 3\t2\n270\tHome Score - Period 4\t2\n272\tHome Score - Period 5\t2\n276\tHome Score - Period 7\t2\n278\tHome Score - Period 8\t2\n280\tHome Score - Period 9\t2\n282\tHome Score - Current Period\t2\n284\tGuest Score - Period 1\t2\n286\tGuest Score - Period 2\t2\n288\tGuest Score - Period 3\t2\n290\tGuest Score - Period 4\t2\n292\tGuest Score - Period 5\t2\n294\tGuest Score - Period 6\t2\n296\tGuest Score - Period 7\t2\n298\tGuest Score - Period 8\t2\n300\tGuest Score - Period 9\t2\n302\tGuest Score - Current Period\t2\n304\tHome In-game Plyr 1-Status\t1\n305\tHome In-game Plyr 1-Number\t2\n307\tHome In-game Plyr 1-Fouls\t2\n309\tHome In-game Plyr 1-Points\t2\n311\tHome In-game Plyr 2-Status\t1\n312\tHome In-game Plyr 2-Number\t2\n314\tHome In-game Plyr 2-Fouls\t2\n316\tHome In-game Plyr 2-Points\t2\n318\tHome In-game Plyr 3-Status\t1\n319\tHome In-game Plyr 3-Number\t2\n321\tHome In-game Plyr 3-Fouls\t2\n323\tHome In-game Plyr 3-Points\t2\n325\tHome In-game Plyr 4-Status\t1\n326\tHome In-game Plyr 4-Number\t2\n328\tHome In-game Plyr 4-Fouls\t2\n330\tHome In-game Plyr 4-Points\t2\n332\tHome In-game Plyr 5-Status\t1\n333\tHome In-game Plyr 5-Number\t2\n335\tHome In-game Plyr 5-Fouls\t2\n337\tHome In-game Plyr 5-Points\t2\n339\tReserved - Home In-game Plyr 6\t7\n346\tHome Roster Plyr 1-Status\t1\n347\tHome Roster Plyr 1-Number\t2\n349\tHome Roster Plyr 1-Fouls\t2\n351\tHome Roster Plyr 1-Points\t2\n353\tHome Roster Plyr 2-Status\t1\n354\tHome Roster Plyr 2-Number\t2\n356\tHome Roster Plyr 2-Fouls\t2\n358\tHome Roster Plyr 2-Points\t2\n360\tHome Roster Plyr 3-Status\t1\n361\tHome Roster Plyr 3-Number\t2\n363\tHome Roster Plyr 3-Fouls\t2\n365\tHome Roster Plyr 3-Points\t2\n367\tHome Roster Plyr 4-Status\t1\n368\tHome Roster Plyr 4-Number\t2\n370\tHome Roster Plyr 4-Fouls\t2\n372\tHome Roster Plyr 4-Points\t2\n374\tHome Roster Plyr 5-Status\t1\n375\tHome Roster Plyr 5-Number\t2\n377\tHome Roster Plyr 5-Fouls\t2\n379\tHome Roster Plyr 5-Points\t2\n381\tHome Roster Plyr 6-Status\t1\n382\tHome Roster Plyr 6-Number\t2\n384\tHome Roster Plyr 6-Fouls\t2\n386\tHome Roster Plyr 6-Points\t2\n388\tHome Roster Plyr 7-Status\t1\n389\tHome Roster Plyr 7-Number\t2\n391\tHome Roster Plyr 7-Fouls\t2\n393\tHome Roster Plyr 7-Points\t2\n395\tHome Roster Plyr 8-Status\t1\n396\tHome Roster Plyr 8-Number\t2\n398\tHome Roster Plyr 8-Fouls\t2\n400\tHome Roster Plyr 8-Points\t2\n402\tHome Roster Plyr 9-Status\t1\n403\tHome Roster Plyr 9-Number\t2\n405\tHome Roster Plyr 9-Fouls\t2\n407\tHome Roster Plyr 9-Points\t2\n409\tHome Roster Plyr 10-Status\t1\n410\tHome Roster Plyr 10-Number\t2\n412\tHome Roster Plyr 10-Fouls\t2\n414\tHome Roster Plyr 10-Points\t2\n416\tHome Roster Plyr 11-Status\t1\n417\tHome Roster Plyr 11-Number\t2\n419\tHome Roster Plyr 11-Fouls\t2\n421\tHome Roster Plyr 11-Points\t2\n423\tHome Roster Plyr 12-Status\t1\n424\tHome Roster Plyr 12-Number\t2\n426\tHome Roster Plyr 12-Fouls\t2\n428\tHome Roster Plyr 12-Points\t2\n430\tHome Roster Plyr 13-Status\t1\n431\tHome Roster Plyr 13-Number\t2\n433\tHome Roster Plyr 13-Fouls\t2\n435\tHome Roster Plyr 13-Points\t2\n437\tHome Roster Plyr 14-Status\t1\n438\tHome Roster Plyr 14-Number\t2\n440\tHome Roster Plyr 14-Fouls\t2\n442\tHome Roster Plyr 14-Points\t2\n444\tHome Roster Plyr 15-Status\t1\n445\tHome Roster Plyr 15-Number\t2\n447\tHome Roster Plyr 15-Fouls\t2\n449\tHome Roster Plyr 15-Points\t2\n451\tHome Assists\t4\n455\tHome Rebounds\t4\n459\tHome Blocked Shots\t4\n463\tHome Steals\t4\n467\tHome Total Hustle (A,R,B,S)\t4\n471\tHome Total Hustle (R,B,S)\t4\n475\tGuest In-game Plyr 1-Status\t1\n476\tGuest In-game Plyr 1-Number\t2\n478\tGuest In-game Plyr 1-Fouls\t2\n480\tGuest In-game Plyr 1-Points\t2\n482\tGuest In-game Plyr 2-Status\t1\n483\tGuest In-game Plyr 2-Number\t2\n485\tGuest In-game Plyr 2-Fouls\t2\n487\tGuest In-game Plyr 2-Points\t2\n489\tGuest In-game Plyr 3-Status\t1\n490\tGuest In-game Plyr 3-Number\t2\n492\tGuest In-game Plyr 3-Fouls\t2\n494\tGuest In-game Plyr 3-Points\t2\n496\tGuest In-game Plyr 4-Status\t1\n497\tGuest In-game Plyr 4-Number\t2\n499\tGuest In-game Plyr 4-Fouls\t2\n501\tGuest In-game Plyr 4-Points\t2\n503\tGuest In-game Plyr 5-Status\t1\n504\tGuest In-game Plyr 5-Number\t2\n506\tGuest In-game Plyr 5-Fouls\t2\n508\tGuest In-game Plyr 5-Points\t2\n510\tReserved - Guest In-game Plyr 6\t7\n517\tGuest Roster Plyr 1-Status\t1\n518\tGuest Roster Plyr 1-Number\t2\n520\tGuest Roster Plyr 1-Fouls\t2\n522\tGuest Roster Plyr 1-Points\t2\n524\tGuest Roster Plyr 2-Status\t1\n525\tGuest Roster Plyr 2-Number\t2\n527\tGuest Roster Plyr 2-Fouls\t2\n529\tGuest Roster Plyr 2-Points\t2\n531\tGuest Roster Plyr 3-Status\t1\n532\tGuest Roster Plyr 3-Number\t2\n534\tGuest Roster Plyr 3-Fouls\t2\n536\tGuest Roster Plyr 3-Points\t2\n538\tGuest Roster Plyr 4-Status\t1\n539\tGuest Roster Plyr 4-Number\t2\n541\tGuest Roster Plyr 4-Fouls\t2\n543\tGuest Roster Plyr 4-Points\t2\n545\tGuest Roster Plyr 5-Status\t1\n546\tGuest Roster Plyr 5-Number\t2\n548\tGuest Roster Plyr 5-Fouls\t2\n550\tGuest Roster Plyr 5-Points\t2\n552\tGuest Roster Plyr 6-Status\t1\n553\tGuest Roster Plyr 6-Number\t2\n555\tGuest Roster Plyr 6-Fouls\t2\n557\tGuest Roster Plyr 6-Points\t2\n559\tGuest Roster Plyr 7-Status\t1\n560\tGuest Roster Plyr 7-Number\t2\n562\tGuest Roster Plyr 7-Fouls\t2\n564\tGuest Roster Plyr 7-Points\t2\n566\tGuest Roster Plyr 8-Status\t1\n567\tGuest Roster Plyr 8-Number\t2\n569\tGuest Roster Plyr 8-Fouls\t2\n571\tGuest Roster Plyr 8-Points\t2\n573\tGuest Roster Plyr 9-Status\t1\n574\tGuest Roster Plyr 9-Number\t2\n576\tGuest Roster Plyr 9-Fouls\t2\n578\tGuest Roster Plyr 9-Points\t2\n580\tGuest Roster Plyr 10-Status\t1\n581\tGuest Roster Plyr 10-Number\t2\n583\tGuest Roster Plyr 10-Fouls\t2\n585\tGuest Roster Plyr 10-Points\t2\n587\tGuest Roster Plyr 11-Status\t1\n588\tGuest Roster Plyr 11-Number\t2\n590\tGuest Roster Plyr 11-Fouls\t2\n592\tGuest Roster Plyr 11-Points\t2\n594\tGuest Roster Plyr 12-Status\t1\n595\tGuest Roster Plyr 12-Number\t2\n597\tGuest Roster Plyr 12-Fouls\t2\n599\tGuest Roster Plyr 12-Points\t2\n601\tGuest Roster Plyr 13-Status\t1\n602\tGuest Roster Plyr 13-Number\t2\n604\tGuest Roster Plyr 13-Fouls\t2\n606\tGuest Roster Plyr 13-Points\t2\n608\tGuest Roster Plyr 14-Status\t1\n609\tGuest Roster Plyr 14-Number\t2\n611\tGuest Roster Plyr 14-Fouls\t2\n613\tGuest Roster Plyr 14-Points\t2\n615\tGuest Roster Plyr 15-Status\t1\n616\tGuest Roster Plyr 15-Number\t2\n618\tGuest Roster Plyr 15-Fouls\t2\n620\tGuest Roster Plyr 15-Points\t2\n622\tGuest Assists\t4\n626\tGuest Rebounds\t4\n630\tGuest Blocked Shots\t4\n634\tGuest Steals\t4\n638\tGuest Total Hustle (A,R,B,S)\t4\n642\tGuest Total Hustle (R,B,S)\t4";
	_DAKData[1] =
		"Football\t295\n1\tMain Clock Time [mm:ss/ss.t]\t5\n6\tMain Clock Time [mm:ss.t]\t8\n14\tMain Clock/Time Out/TOD [mm:ss/ss.t]\t5\n19\tMain Clock/Time Out/TOD [mm:ss.t]\t8\n27\tMain Clock =0\t1\n28\tMain Clock Stopped\t1\n29\tMain Clock/Time Out Horn\t1\n30\tMain Clock Horn\t1\n31\tTime Out Horn\t1\n32\tTime Out Time\t8\n40\tTime of Day\t8\n48\tHome Team Name\t20\n68\tGuest Team Name\t20\n88\tHome Team Abbreviation\t10\n98\tGuest Team Abbreviation\t10\n108\tHome Team Score\t4\n112\tGuest Team Score\t4\n116\tHome Time Outs Left - Full\t2\n118\tHome Time Outs Left - Partial\t2\n120\tHome Time Outs Left - Television\t2\n122\tHome Time Outs Left - Total\t2\n124\tGuest Time Outs Left - Full\t2\n126\tGuest Time Outs Left - Partial\t2\n128\tGuest Time Outs Left - Television\t2\n130\tGuest Time Outs Left - Total\t2\n132\tHome Time Out Indicator\t1\n133\tHome Time Out Text\t4\n137\tGuest Time Out Indicator\t1\n138\tGuest Time Out Text\t4\n142\tQuarter\t2\n144\tQuarter Text\t4\n148\tQuarter Description\t12\n160\tInternal Relay\t1\n161\tAd Panel / Caption Power\t1\n162\tAd Panel / Caption #1\t1\n163\tAd Panel / Caption #2 \t1\n164\tAd Panel / Caption #3\t1\n165\tAd Panel / Caption #4\t1\n166\tReserved for Future Use\t35\n201\tPlay Clock Time\t8\n209\tPlay Clock Horn\t1\n210\tHome Possession Indicator\t1\n211\tHome Possession Text\t4\n215\tGuest Possession Indicator\t1\n216\tGuest Possession Text\t4\n220\tBall On\t2\n222\tDown\t3\n225\tTo Go\t2\n227\tHome Score - Period 1\t2\n229\tHome Score - Period 2\t2\n231\tHome Score - Period 3\t2\n233\tHome Score - Period 4\t2\n235\tHome Score - Period 5\t2\n237\tHome Score - Period 6\t2\n239\tHome Score - Period 7\t2\n241\tHome Score - Period 8\t2\n243\tHome Score - Period 9\t2\n245\tHome Score - Current Period\t2\n247\tGuest Score - Period 1\t2\n249\tGuest Score - Period 2\t2\n251\tGuest Score - Period 3\t2\n253\tGuest Score - Period 4\t2\n255\tGuest Score - Period 5\t2\n257\tGuest Score - Period 6\t2\n259\tGuest Score - Period 7\t2\n261\tGuest Score - Period 8\t2\n263\tGuest Score - Period 9\t2\n265\tGuest Score - Current Period\t2\n267\tHome Rushing Yards\t4\n271\tHome Passing Yards\t4\n275\tHome Total Yards\t4\n279\tGuest Rushing Yards\t4\n283\tGuest Passing Yards\t4\n287\tGuest Total Yards\t4\n291\tHome First Downs\t2\n293\tGuest First Downs\t2";
	_DAKData[2] =
		"Hockey/Lacrosse\t493\n1\tMain Clock Time (mm:ss/ss.t )\t5\n6\tMain Clock Time (mm:ss.t )\t8\n14\tMain Clock/Time Out/TOD (mm:ss/ss.t )\t5\n19\tMain Clock/Time Out/TOD (mm:ss.t )\t8\n27\tMain Clock 0\t1\n28\tMain Clock Stopped\t1\n29\tMain Clock/Time Out Horn\t1\n30\tMain Clock Horn\t1\n31\tTime Out Horn\t1\n32\tTime Out Time (mm:ss)\t8\n40\tTime of Day (hh:mm:ss)\t8\n48\tHome Team Name\t20\n68\tGuest Team Name\t20\n88\tHome Team Abbreviation\t10\n98\tGuest Team Abbreviation\t10\n108\tHome Team Score\t4\n112\tGuest Team Score\t4\n116\tHome Time Outs Left - Full\t2\n118\tHome Time Outs Left - Partial\t2\n120\tHome Time Outs Left - Television\t2\n122\tHome Time Outs Left - Total\t2\n124\tGuest Time Outs Left - Full\t2\n126\tGuest Time Outs Left - Partial\t2\n128\tGuest Time Outs Left - Television\t2\n130\tGuest Time Outs Left - Total\t2\n132\tHome Time Out Indicator\t1\n133\tHome Time Out Text\t4\n137\tGuest Time Out Indicator\t1\n138\tGuest Time Out Text\t4\n142\tPeriod\t2\n144\tPeriod Text\t4\n148\tPeriod Description\t12\n160\tInternal Relay\t1\n161\tAd Panel / Caption Power\t1\n162\tAd Panel / Caption #1\t1\n163\tAd Panel / Caption #2\t1\n164\tAd Panel / Caption #3\t1\n165\tAd Panel / Caption #4\t1\n166\tReserved for Future Use\t35\n201\tShot Clock Time (mm:ss)\t8\n209\tShot Clock Horn\t1\n210\tInverse Time Clock (mm:ss)\t8\n218\tInverse/Main/Time Out/TOD (mm:ss)\t8\n226\tHome Player #1-Number\t2\n228\tHome Player #1-Penalty Time (mm:ss)\t8\n236\tHome Player #2-Number\t2\n238\tHome Player #2-Penalty Time (mm:ss)\t8\n246\tHome Player #3-Number\t2\n248\tHome Player #3-Penalty Time (mm:ss)\t8\n256\tHome Player #4-Number\t2\n258\tHome Player #4-Penalty Time (mm:ss)\t8\n266\tHome Player #5-Number\t2\n268\tHome Player #5-Penalty Time (mm:ss)\t8\n276\tHome Player #6-Number\t2\n278\tHome Player #6-Penalty Time (mm:ss)\t8\n286\tGuest Player #1-Number\t2\n288\tGuest Player #1-Penalty Time (mm:ss)\t8\n296\tGuest Player #2-Number\t2\n298\tGuest Player #2-Penalty Time (mm:ss)\t8\n306\tGuest Player #3-Number\t2\n308\tGuest Player #3-Penalty Time (mm:ss)\t8\n316\tGuest Player #4-Number\t2\n318\tGuest Player #4-Penalty Time (mm:ss)\t8\n326\tGuest Player #5-Number\t2\n328\tGuest Player #5-Penalty Time (mm:ss)\t8\n336\tGuest Player #6-Number\t2\n338\tGuest Player #6-Penalty Time (mm:ss)\t8\n346\tHome Penalty Indicator\t1\n347\tHome Penalty Text\t7\n354\tGuest Penalty Indicator\t1\n355\tGuest Penalty Text\t7\n362\tHome Score - Period 1\t2\n364\tHome Score - Period 2\t2\n366\tHome Score - Period 3\t2\n368\tHome Score - Period 4\t2\n370\tHome Score - Period 5\t2\n372\tHome Score - Period 6\t2\n374\tHome Score - Period 7\t2\n376\tHome Score - Period 8\t2\n378\tHome Score - Period 9\t2\n380\tHome Score - Current Period\t2\n382\tGuest Score - Period 1\t2\n384\tGuest Score - Period 2\t2\n386\tGuest Score - Period 3\t2\n388\tGuest Score - Period 4\t2\n390\tGuest Score - Period 5\t2\n392\tGuest Score - Period 6\t2\n394\tGuest Score - Period 7\t2\n396\tGuest Score - Period 8\t2\n398\tGuest Score - Period 9\t2\n400\tGuest Score - Current Period\t2\n402\tHome Shots On Goal - Period 1\t2\n404\tHome Shots On Goal - Period 2\t2\n406\tHome Shots On Goal - Period 3\t2\n408\tHome Shots On Goal - Period 4\t2\n410\tHome Shots On Goal - Period 5\t2\n412\tHome Shots On Goal - Period 6\t2\n414\tHome Shots On Goal - Period 7\t2\n416\tHome Shots On Goal - Period 8\t2\n418\tHome Shots On Goal - Period 9\t2\n420\tHome Shots On Goal - Current\t2\n422\tHome Shots On Goal - Total\t3\n425\tHome Saves - Period 1\t2\n427\tHome Saves - Period 2\t2\n429\tHome Saves - Period 3\t2\n431\tHome Saves - Period 4\t2\n433\tHome Saves - Period 5\t2\n435\tHome Saves - Period 6\t2\n437\tHome Saves - Period 7\t2\n439\tHome Saves - Period 8\t2\n441\tHome Saves - Period 9\t2\n443\tHome Saves - Current\t2\n445\tHome Saves - Total\t3\n448\tGuest Shots On Goal - Period 1\t2\n450\tGuest Shots On Goal - Period 2\t2\n452\tGuest Shots On Goal - Period 3\t2\n454\tGuest Shots On Goal - Period 4\t2\n456\tGuest Shots On Goal - Period 5\t2\n458\tGuest Shots On Goal - Period 6\t2\n460\tGuest Shots On Goal - Period 7\t2\n462\tGuest Shots On Goal - Period 8\t2\n464\tGuest Shots On Goal - Period 9\t2\n466\tGuest Shots On Goal - Current\t2\n468\tGuest Shots On Goal - Total\t3\n471\tGuest Saves - Period 1\t2\n473\tGuest Saves - Period 2\t2\n475\tGuest Saves - Period 3\t2\n477\tGuest Saves - Period 4\t2\n479\tGuest Saves - Period 5\t2\n481\tGuest Saves - Period 6\t2\n483\tGuest Saves - Period 7\t2\n485\tGuest Saves - Period 8\t2\n487\tGuest Saves - Period 9\t2\n489\tGuest Saves - Current\t2\n491\tGuest Saves - Total\t3";
	_DAKData[3] =
		"Volleyball\t596\n1\tMain Clock Time (mm:ss/ss.t)\t5\n6\tMain Clock Time (mm:ss.t)\t8\n14\tMain Clock/Time Out/TOD (mm:ss/ss.t)\t5\n19\tMain Clock/Time Out/TOD (mm:ss.t)\t8\n27\tMain Clock =0\t1\n28\tMain Clock Stopped\t1\n29\tMain Clock/Time Out Horn\t1\n30\tMain Clock Horn\t1\n31\tTime Out Horn\t1\n32\tTime Out Time (mm:ss)\t8\n40\tTime of Day (hh:mm:ss)\t8\n48\tHome Team Name\t20\n68\tGuest Team Name\t20\n88\tHome Team Abbreviation\t10\n98\tGuest Team Abbreviation\t10\n108\tHome Team Score\t4\n112\tGuest Team Score\t4\n116\tHome Time Outs Left - Full\t2\n118\tHome Time Outs Left - Partial\t2\n120\tHome Time Outs Left - Television\t2\n122\tHome Time Outs Left - Total\t2\n124\tGuest Time Outs Left - Full\t2\n126\tGuest Time Outs Left - Partial\t2\n128\tGuest Time Outs Left - Television\t2\n130\tGuest Time Outs Left - Total\t2\n132\tHome Time Out Indicator\t1\n133\tHome Time Out Text\t4\n137\tGuest Time Out Indicator\t1\n138\tGuest Time Out Text\t4\n142\tGame\t2\n144\tGame Text\t4\n148\tGame Description\t12\n160\tInternal Relay\t1\n161\tAd Panel / Caption Power\t1\n162\tAd Panel / Caption #1\t1\n163\tAd Panel / Caption #2\t1\n164\tAd Panel / Caption #3\t1\n165\tAd Panel / Caption #4\t1\n166\tReserved for Future Use\t35\n201\tHome Serve Indicator\t1\n202\tHome Serve Arrow\t1\n203\tHome Serve Text\t5\n208\tGuest Serve Indicator\t1\n209\tGuest Serve Arrow\t1\n210\tGuest Serve Text\t5\n215\tHome Games Won\t2\n217\tGuest Games Won\t2\n219\tMatch Number\t3\n222\tHome Score - Game 1\t2\n224\tHome Score - Game 2\t2\n226\tHome Score - Game 3\t2\n228\tHome Score - Game 4\t2\n230\tHome Score - Game 5\t2\n232\tHome Score - Game 6\t2\n234\tHome Score - Game 7\t2\n236\tHome Score - Game 8\t2\n238\tHome Score - Game 9\t2\n240\tHome Score - Current Game\t2\n242\tGuest Score - Game 1\t2\n244\tGuest Score - Game 2\t2\n246\tGuest Score - Game 3\t2\n248\tGuest Score - Game 4\t2\n250\tGuest Score - Game 5\t2\n252\tGuest Score - Game 6\t2\n254\tGuest Score - Game 7\t2\n256\tGuest Score - Game 8\t2\n258\tGuest Score - Game 9\t2\n260\tGuest Score - Current Game\t2\n262\tHome In-game Plyr 1-Status\t1\n263\tHome In-game Plyr 1-Number\t2\n265\tHome In-game Plyr 1-User Defined 1\t2\n267\tHome In-game Plyr 1-User Defined 2\t2\n269\tHome In-game Plyr 2-Status\t1\n270\tHome In-game Plyr 2-Number\t2\n272\tHome In-game Plyr 2-User Defined 1\t2\n274\tHome In-game Plyr 2-User Defined 2\t2\n276\tHome In-game Plyr 3-Status\t1\n277\tHome In-game Plyr 3-Number\t2\n279\tHome In-game Plyr 3-User Defined 1\t2\n281\tHome In-game Plyr 3-User Defined 2\t2\n283\tHome In-game Plyr 4-Status\t1\n284\tHome In-game Plyr 4-Number\t2\n286\tHome In-game Plyr 4-User Defined 1\t2\n288\tHome In-game Plyr 4-User Defined 2\t2\n290\tHome In-game Plyr 5-Status\t1\n291\tHome In-game Plyr 5-Number\t2\n293\tHome In-game Plyr 5-User Defined 1\t2\n295\tHome In-game Plyr 5-User Defined 2\t2\n297\tHome In-game Plyr 6-Status\t1\n298\tHome In-game Plyr 6-Number\t2\n300\tHome In-game Plyr 6-User Defined 1\t2\n302\tHome In-game Plyr 6-User Defined 2\t2\n304\tHome Roster Plyr 1-Status\t1\n305\tHome Roster Plyr 1-Number\t2\n307\tHome Roster Plyr 1-User Defined 1\t2\n309\tHome Roster Plyr 1-User Defined 2\t2\n311\tHome Roster Plyr 2-Status\t1\n312\tHome Roster Plyr 2-Number\t2\n314\tHome Roster Plyr 2-User Defined 1\t2\n316\tHome Roster Plyr 2-User Defined 2\t2\n318\tHome Roster Plyr 3-Status\t1\n319\tHome Roster Plyr 3-Number\t2\n321\tHome Roster Plyr 3-User Defined 1\t2\n323\tHome Roster Plyr 3-User Defined 2\t2\n325\tHome Roster Plyr 4-Status\t1\n326\tHome Roster Plyr 4-Number\t2\n328\tHome Roster Plyr 4-User Defined 1\t2\n330\tHome Roster Plyr 4-User Defined 2\t2\n332\tHome Roster Plyr 5-Status\t1\n333\tHome Roster Plyr 5-Number\t2\n335\tHome Roster Plyr 5-User Defined 1\t2\n337\tHome Roster Plyr 5-User Defined 2\t2\n339\tHome Roster Plyr 6-Status\t1\n340\tHome Roster Plyr 6-Number\t2\n342\tHome Roster Plyr 6-User Defined 1\t2\n344\tHome Roster Plyr 6-User Defined 2\t2\n346\tHome Roster Plyr 7-Status\t1\n347\tHome Roster Plyr 7-Number\t2\n349\tHome Roster Plyr 7-User Defined 1\t2\n351\tHome Roster Plyr 7-User Defined 2\t2\n353\tHome Roster Plyr 8-Status\t1\n354\tHome Roster Plyr 8-Number\t2\n356\tHome Roster Plyr 8-User Defined 1\t2\n358\tHome Roster Plyr 8-User Defined 2\t2\n360\tHome Roster Plyr 9-Status\t1\n361\tHome Roster Plyr 9-Number\t2\n363\tHome Roster Plyr 9-User Defined 1\t2\n365\tHome Roster Plyr 9-User Defined 2\t2\n367\tHome Roster Plyr 10-Status\t1\n368\tHome Roster Plyr 10-Number\t2\n370\tHome Roster Plyr 10-User Defined 1\t2\n372\tHome Roster Plyr 10-User Defined 2\t2\n374\tHome Roster Plyr 11-Status\t1\n375\tHome Roster Plyr 11-Number\t2\n377\tHome Roster Plyr 11-User Defined 1\t2\n379\tHome Roster Plyr 11-User Defined 2\t2\n381\tHome Roster Plyr 12-Status\t1\n382\tHome Roster Plyr 12-Number\t2\n384\tHome Roster Plyr 12-User Defined 1\t2\n386\tHome Roster Plyr 12-User Defined 2\t2\n388\tHome Roster Plyr 13-Status\t1\n389\tHome Roster Plyr 13-Number\t2\n391\tHome Roster Plyr 13-User Defined 1\t2\n393\tHome Roster Plyr 13-User Defined 2\t2\n395\tHome Roster Plyr 14-Status\t1\n396\tHome Roster Plyr 14-Number\t2\n398\tHome Roster Plyr 14-User Defined 1\t2\n400\tHome Roster Plyr 14-User Defined 2\t2\n402\tHome Roster Plyr 15-Status\t1\n403\tHome Roster Plyr 15-Number\t2\n405\tHome Roster Plyr 15-User Defined 1\t2\n407\tHome Roster Plyr 15-User Defined 2\t2\n409\tHome Aces\t4\n413\tHome Kills\t4\n417\tHome Blocks\t4\n421\tHome Digs\t4\n425\tHome Total Hustle\t4\n429\tGuest In-game Plyr 1-Status\t1\n430\tGuest In-game Plyr 1-Number\t2\n432\tGuest In-game Plyr 1-User Defined 1\t2\n434\tGuest In-game Plyr 1-User Defined 2\t2\n436\tGuest In-game Plyr 2-Status\t1\n437\tGuest In-game Plyr 2-Number\t2\n439\tGuest In-game Plyr 2-User Defined 1\t2\n441\tGuest In-game Plyr 2-User Defined 2\t2\n443\tGuest In-game Plyr 3-Status\t1\n444\tGuest In-game Plyr 3-Number\t2\n446\tGuest In-game Plyr 3-User Defined 1\t2\n448\tGuest In-game Plyr 3-User Defined 2\t2\n450\tGuest In-game Plyr 4-Status\t1\n451\tGuest In-game Plyr 4-Number\t2\n453\tGuest In-game Plyr 4-User Defined 1\t2\n455\tGuest In-game Plyr 4-User Defined 2\t2\n457\tGuest In-game Plyr 5-Status\t1\n458\tGuest In-game Plyr 5-Number\t2\n460\tGuest In-game Plyr 5-User Defined 1\t2\n462\tGuest In-game Plyr 5-User Defined 2\t2\n464\tGuest In-game Plyr 6-Status\t1\n465\tGuest In-game Plyr 6-Number\t2\n467\tGuest In-game Plyr 6-User Defined 1\t2\n469\tGuest In-game Plyr 6-User Defined 2\t2\n471\tGuest Roster Plyr 1-Status\t1\n472\tGuest Roster Plyr 1-Number\t2\n474\tGuest Roster Plyr 1-User Defined 1\t2\n476\tGuest Roster Plyr 1-User Defined 2\t2\n478\tGuest Roster Plyr 2-Status\t1\n479\tGuest Roster Plyr 2-Number\t2\n481\tGuest Roster Plyr 2-User Defined 1\t2\n483\tGuest Roster Plyr 2-User Defined 2\t2\n485\tGuest Roster Plyr 3-Status\t1\n486\tGuest Roster Plyr 3-Number\t2\n488\tGuest Roster Plyr 3-User Defined 1\t2\n490\tGuest Roster Plyr 3-User Defined 2\t2\n492\tGuest Roster Plyr 4-Status\t1\n493\tGuest Roster Plyr 4-Number\t2\n495\tGuest Roster Plyr 4-User Defined 1\t2\n497\tGuest Roster Plyr 4-User Defined 2\t2\n499\tGuest Roster Plyr 5-Status\t1\n500\tGuest Roster Plyr 5-Number\t2\n502\tGuest Roster Plyr 5-User Defined 1\t2\n504\tGuest Roster Plyr 5-User Defined 2\t2\n506\tGuest Roster Plyr 6-Status\t1\n507\tGuest Roster Plyr 6-Number\t2\n509\tGuest Roster Plyr 6-User Defined 1\t2\n511\tGuest Roster Plyr 6-User Defined 2\t2\n513\tGuest Roster Plyr 7-Status\t1\n514\tGuest Roster Plyr 7-Number\t2\n516\tGuest Roster Plyr 7-User Defined 1\t2\n518\tGuest Roster Plyr 7-User Defined 2\t2\n520\tGuest Roster Plyr 8-Status\t1\n521\tGuest Roster Plyr 8-Number\t2\n523\tGuest Roster Plyr 8-User Defined 1\t2\n525\tGuest Roster Plyr 8-User Defined 2\t2\n527\tGuest Roster Plyr 9-Status\t1\n528\tGuest Roster Plyr 9-Number\t2\n530\tGuest Roster Plyr 9-User Defined 1\t2\n532\tGuest Roster Plyr 9-User Defined 2\t2\n534\tGuest Roster Plyr 10-Status\t1\n535\tGuest Roster Plyr 10-Number\t2\n537\tGuest Roster Plyr 10-User Defined 1\t2\n539\tGuest Roster Plyr 10-User Defined 2\t2\n541\tGuest Roster Plyr 11-Status\t1\n542\tGuest Roster Plyr 11-Number\t2\n544\tGuest Roster Plyr 11-User Defined 1\t2\n546\tGuest Roster Plyr 11-User Defined 2\t2\n548\tGuest Roster Plyr 12-Status\t1\n549\tGuest Roster Plyr 12-Number\t2\n551\tGuest Roster Plyr 12-User Defined 1\t2\n553\tGuest Roster Plyr 12-User Defined 2\t2\n555\tGuest Roster Plyr 13-Status\t1\n556\tGuest Roster Plyr 13-Number\t2\n558\tGuest Roster Plyr 13-User Defined 1\t2\n560\tGuest Roster Plyr 13-User Defined 2\t2\n562\tGuest Roster Plyr 14-Status\t1\n563\tGuest Roster Plyr 14-Number\t2\n565\tGuest Roster Plyr 14-User Defined 1\t2\n567\tGuest Roster Plyr 14-User Defined 2\t2\n569\tGuest Roster Plyr 15-Status\t1\n570\tGuest Roster Plyr 15-Number\t2\n572\tGuest Roster Plyr 15-User Defined 1\t2\n574\tGuest Roster Plyr 15-User Defined 2\t2\n576\tGuest Aces\t4\n580\tGuest Kills\t4\n584\tGuest Blocks\t4\n588\tGuest Digs\t4\n592\tGuest Total Hustle\t4";

	for (std::string _dataLine : _DAKData) {
		std::stringstream streamSportData(_dataLine);

		while (readHeaderLine(streamSportData, DAKTSVHeader sportDataLine) == 0) {
			DAKSportData *sportData = new DAKSportData(sportDataLine.sport, sportDataLine.size);

			while (readDataLine(streamSportData, DAKTSVData fieldDataLine) == 0) {
				sportData->AddFieldData(fieldDataLine.index, fieldDataLine.field, fieldDataLine.length);
			}

			_allSportsData[sportData->GetSportName()] = sportData;
		}
	}
}

void DAKDataUtils::clearSportsData()
{
	for (std::pair<std::string, DAKSportData *> sportData : _allSportsData) {
		delete sportData.second;
	}

	_allSportsData.clear();
}

DAKSportData *DAKDataUtils::getSportData(std::string sport)
{
	for (std::pair<std::string, DAKSportData *> sportData : _allSportsData) {
		if (sportData.first == sport)
			return sportData.second;
	}

	return nullptr;
}

void DAKDataUtils::PopulateSportProps(obs_property_t *sportList)
{
	for (std::pair<std::string, DAKSportData *> sportData : _allSportsData) {
		obs_property_list_add_string(sportList, sportData.first.c_str(), sportData.first.c_str());
	}
}

void DAKDataUtils::AddFilter(DAKFilter *newFilter)
{
	uint32_t index = newFilter->GetIndex();
	_filters[index].push_back(newFilter);
}

void DAKDataUtils::RemoveFilter(DAKFilter *oldFilter)
{
	uint32_t index = oldFilter->GetIndex();
	std::vector<DAKFilter *> filters = _filters[index];

	for (auto it = filters.begin(); it != filters.end(); it++) {
		if (*it == oldFilter) {
			delete (*it);
			filters.erase(it);
			break;
		}
	}
}

void DAKDataUtils::UpdateField(uint32_t index, std::string value)
{
	std::vector<DAKFilter *> filters = _filters[index];

	for (DAKFilter *filterData : filters) {
		filterData->SetValue(value);
	}
}

/*
 * =========================================================================
 * GLOBAL TICK LOGIC (Executed Once Per Frame)
 * =========================================================================
 */

// Call this once when the plugin loads (e.g., in obs_module_load).
void DAKDataUtils::sync_init()
{
	serial = SerialPort::create();

	// Set callbacks (will be invoked in main thread)
	serial->setLineReceivedCallback(onLineReceived);
	serial->setErrorCallback(onError);
}

// Call this once when the plugin unloads (e.g., in obs_module_unload).
void DAKDataUtils::sync_destroy()
{
	// Destroys the mutex object when the plugin is unloaded.
	serial->stopReading();
	serial->close();

	blog(LOG_INFO, "OBS global tick synchronization destroyed.");
}

/**
 * @brief This function contains the core logic that should run exactly once
 * per OBS video frame, regardless of the number of active plugin instances.
 */
void DAKDataUtils::execute_global_tick_logic(void *data, uint32_t width, uint32_t height)
{
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(width);
	UNUSED_PARAMETER(height);

	// --- CORE SINGLE-RUN LOGIC GOES HERE ---
	// This block will execute exactly once per global OBS frame tick.

	if (SerialPort::hasPendingSignals()) {
		serial->processSignals();
	}
}

void DAKDataUtils::startSerial(std::string port)
{
	// Open and start reading
	if (serial->open(port, SerialPort::BAUD_19200, '\n')) {
		serial->startReading();
	}
}

std::string DAKDataUtils::getSerialPort()
{
	if (serial)
		return serial->getPort();
	else
		return "";
}

void onLineReceived(const std::string &line)
{
	// This runs in the MAIN thread - safe to update UI, call non-thread-safe code
	std::cout << "Main thread received: " << line << std::endl;
	// Your consumer library processes the line here

	//=================================  process data line and update fields accordingly =====================//
}

void onError(const std::string &error)
{
	std::cerr << "Main thread error: " << error << std::endl;
}
