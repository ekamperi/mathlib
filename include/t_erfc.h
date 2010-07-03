#ifndef __T_ERFC_H__
#define __T_ERFC_H__

const struct
t1dentry {
	double x;	/* Input */
	double y;	/* Output */
} t1dtable[] = {
	{ -1.9095999401414304e-01,  1.2128845310910534e+00 },
	{  3.9855949501006585e-02,  9.5505117850912580e-01 },
	{ -4.4796689545114343e-02,  1.0505138595283947e+00 },
	{  3.9201000034364969e-02,  9.5578905598146546e-01 },
	{ -1.1767973179313089e-03,  1.0013278729644783e+00 },
	{  1.3637857014267759e+01,  6.9277413497612510e-83 },
	{  6.1224316030042041e-03,  9.9309166204507926e-01 },
	{  2.2494057285670632e+01,  4.5026928558023153e-222 },
	{  1.4878865997735298e-03,  9.9832110099673421e-01 },
	{ -3.7947546292910683e+00,  1.9999999197623690e+00 },
	{  6.3902557330740841e-03,  9.9278946670678114e-01 },
	{  4.9779023608512457e-01,  4.8144416820665648e-01 },
	{  6.2785911320665092e-02,  9.2924666941298995e-01 },
	{ -9.4839602818361804e-02,  1.1066950452624591e+00 },
	{ -1.6028228150935026e-01,  1.1793222691034417e+00 },
	{ -2.3415445248019520e-03,  1.0026421452318004e+00 },
	{  2.2264994309331968e+01,  1.2899544092526066e-217 },
	{  2.3703789937124846e-02,  9.7325814582633030e-01 },
	{  6.0407676169568242e-02,  9.3192005661754695e-01 },
	{ -1.5520488859706364e-03,  1.0017512982230348e+00 }
};
const struct
t1ldentry {
	long double x;   /* Input */
	long double y;   /* Output */
} t1ldtable[] = {
	{  2.46472715755318031019502095658318952e-03L,  9.97218858854543130528207822704800378e-01L },
	{  1.13981960688342590544351834273839597e-02L,  9.87139069975050137005414402358027814e-01L },
	{  1.73769280181757139111791560237563203e+00L,  1.39919992204349764067952232964031634e-02L },
	{  4.87165425645892545742678125808566847e-01L,  4.90850987220701697908917249257854110e-01L },
	{ -8.34900064761975117856866897281253870e-02L,  1.09398994522939276810742648127217080e+00L },
	{ -1.54888040993113343356257782801321810e+01L,  2.00000000000000000000000000000000000e+00L },
	{ -1.11989282420644734547655700911822363e-03L,  1.00126366320393570192356230696972830e+00L },
	{  2.70254600497577219218869881700584301e-01L,  7.02314711901977312331381286814036002e-01L },
	{  1.65632000881175733191050941650601658e+00L,  1.91606699064151029396276156024914172e-02L },
	{ -1.60868802402311812378525590627731390e+00L,  1.97709577505537391903852467711644181e+00L },
	{  7.54083309203907080505739830324218875e-03L,  9.91491242317834339141998517552778480e-01L },
	{  3.00912583435814885625419643974676198e-02L,  9.66055896561412565191201418102195930e-01L },
	{  2.28106107962492506862663133038182650e+00L,  1.25573927143792078427764641887845465e-03L },
	{ -1.52568854406233824559941655634939806e-03L,  1.00172155383282785151132648904237499e+00L },
	{ -1.26591721804979002366374252240913134e-03L,  1.00142843385307100533172797174330526e+00L },
	{ -3.09367687336461873810929124051227745e+00L,  1.99998786341901453134182325976908601e+00L },
	{  3.81037994012064183050646020388396717e-03L,  9.95700467465156829644391117684065762e-01L },
	{  2.16134476606040984095072690629457668e-01L,  7.59863297277078179013665160113077945e-01L },
	{  1.46023249162168926744000498930964316e-02L,  9.83524211814236657682938846924969312e-01L },
	{ -1.34892335826496055845632321412708166e+00L,  1.94356556339933960549006680684769321e+00L }
};
#endif	/* ! __T_ERFC_H__ */
