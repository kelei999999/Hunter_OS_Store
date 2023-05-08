/* Kelei999999(WangLiang) all rights reserved.  You may use this software
 * and any derivatives exclusively with Kelei999999(WangLiang) products.
 *
 * THIS SOFTWARE IS SUPPLIED BY Kelei999999(WangLiang) "AS IS".  NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH Kelei999999(WangLiang) PRODUCTS, COMBINATION
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * IN NO EVENT WILL Kelei999999(WangLiang) BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF Kelei999999(WangLiang) HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE
 * FULLEST EXTENT ALLOWED BY LAW, Kelei999999(WangLiang)'S TOTAL LIABILITY ON ALL CLAIMS
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF
 * ANY, THAT YOU HAVE PAID DIRECTLY TO Kelei999999(WangLiang) FOR THIS SOFTWARE.
 *
 * Kelei999999(WangLiang) PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
 * TERMS.
 */

/*
 * File: Frame.h
 * Author: Kelei999999(WangLiang)
 * Created on: 2016年2月16日
 * Revision history: 1.1
 */

/*
 * This is a guard condition so that contents of this file are not included
 * more than once.
 */

/*
 * 本框架本质上是一个微型操作系统，并且定义了一组通用的参数组集合，即为：
 * SYS_Parameter，该结构体数组的成员定义参照结构体system_parameter。所有的
 * 用户应用程式将变成围绕着这一参数组集进行读、写和解释。任何一个应用(APP),必须定义
 * 这么一个参数组集，但是每一个成员的解释可以依据不同的应用进行不同的解释，其解释的格
 * 式请参照《模拟量列表》。
 *
 * 本框架使用了Hunter_OS系统作为调度，调度功能可以用其他的操作系统替代。。
 *
 * 使用注意事项：
 * 	1、一定要将void (*Os_us_Time_Base)(uint32 *DATA)和
 * void (*Os_ms_Time_Base)(uint32 *DATA)两个函数分别注册给一个100uS
 * 和1ms定时器的中断函数，具体的时间间隔可以根据应用的实际情况分别设置，建议设置
 * 成100us和1ms；
 * 	2、一定要将void (*Os_ms_Time_Gape)(uint32 *DATA)拷贝到主函数中予
 * 	以执行，否则不会生成时隙。
 */
#ifndef FRAME_H_
#define FRAME_H_
//#include "APP.h"
//----------------------------------------------以下为系统头文件
#include "Typedef.h"
//#include "Hunter_OS.h"
//----------------------------------------------
typedef struct
{
	/*用户应用层初始化函数指针，指向用户初始化函数*/
	void (*Os_APP_Init)(void);
	/*系统调度初始化函数指针，指向系统初始化函数*/
	void (*Os_Init)(void);
	/*存储器写入函数*/
	int8 (*Os_Store_Write)(uint16 address,uint32 data);
	/*存储器读取函数*/
	uint32 (*Os_Store_read)(uint16 address);
	/*基本硬件初始化函数*/
	void (*Os_HardwareBasic_Init)(void);
}frame;
extern volatile frame FRAME;
/*
 * 定义框架结构体数组指针
 */
struct system_parameter
{
	uint32 *value;				//当前值
	const uint32 *lower; 		//下限
	const uint32 *upper;		//上限
	const uint32 *defaultValue;	//默认值
	const uint8 *pp;			//属性
	const uint8 *sign;			//符号
};
/*
 * 定义框架结构体数组指针
 */
struct csystem_parameter
{
	const uint32 *value;		//当前值
	const uint32 *lower; 		//下限
	const uint32 *upper;		//上限
	const uint32 *defaultValue;	//默认值
	const uint8 *pp;			//属性
	const uint8 *sign;			//符号
};
/*
 * 定义框架结构体数组长度
 */
#define System_Parameter_LEN		400
//-------------------------------------------------------------------定义系统运行参数数组，数组的每一个成员的作用请参见《系统模拟量表》
extern struct system_parameter SYS_Parameter;
#define system_parameter_DEFAULTS {RAW_SYSDATA,Lower_Limit,Upper_Limit,Defaults,Attribute_Words,Sign_Bit}
extern uint32 RAW_SYSDATA[System_Parameter_LEN];
extern const uint32 Lower_Limit[System_Parameter_LEN];
extern const uint32 Upper_Limit[System_Parameter_LEN];
extern const uint32 Defaults[System_Parameter_LEN];
extern const uint8 Attribute_Words[System_Parameter_LEN];
extern const uint8 Sign_Bit[System_Parameter_LEN];
/*
 * 参数宏定义
 */
#define				EEPROMINITCode		0x5555
#define				EEPROMINITCode1		0xAAAAAAAA
#define				EEPROMINITCode2		0x13572468
#define				EEPROMINIT			(System_Parameter_LEN + 0)
#define				EEPROMINIT1			(System_Parameter_LEN + 1)
#define				EEPROMINIT2			(System_Parameter_LEN + 2)
#define				Parameter0			0
#define				Parameter1			1
#define				Parameter2			2
#define				Parameter3			3
#define				Parameter4			4
#define				Parameter5			5
#define				Parameter6			6
#define				Parameter7			7
#define				Parameter8			8
#define				Parameter9			9
#define				Parameter10			10
#define				Parameter11			11
#define				Parameter12			12
#define				Parameter13			13
#define				Parameter14			14
#define				Parameter15			15
#define				Parameter16			16
#define				Parameter17			17
#define				Parameter18			18
#define				Parameter19			19
#define				Parameter20			20
#define				Parameter21			21
#define				Parameter22			22
#define				Parameter23			23
#define				Parameter24			24
#define				Parameter25			25
#define				Parameter26			26
#define				Parameter27			27
#define				Parameter28			28
#define				Parameter29			29
#define				Parameter30			30
#define				Parameter31			31
#define				Parameter32			32
#define				Parameter33			33
#define				Parameter34			34
#define				Parameter35			35
#define				Parameter36			36
#define				Parameter37			37
#define				Parameter38			38
#define				Parameter39			39
#define				Parameter40			40
#define				Parameter41			41
#define				Parameter42			42
#define				Parameter43			43
#define				Parameter44			44
#define				Parameter45			45
#define				Parameter46			46
#define				Parameter47			47
#define				Parameter48			48
#define				Parameter49			49
#define				Parameter50			50
#define				Parameter51			51
#define				Parameter52			52
#define				Parameter53			53
#define				Parameter54			54
#define				Parameter55			55
#define				Parameter56			56
#define				Parameter57			57
#define				Parameter58			58
#define				Parameter59			59
#define				Parameter60			60
#define				Parameter61			61
#define				Parameter62			62
#define				Parameter63			63
#define				Parameter64			64
#define				Parameter65			65
#define				Parameter66			66
#define				Parameter67			67
#define				Parameter68			68
#define				Parameter69			69
#define				Parameter70			70
#define				Parameter71			71
#define				Parameter72			72
#define				Parameter73			73
#define				Parameter74			74
#define				Parameter75			75
#define				Parameter76			76
#define				Parameter77			77
#define				Parameter78			78
#define				Parameter79			79
#define				Parameter80			80
#define				Parameter81			81
#define				Parameter82			82
#define				Parameter83			83
#define				Parameter84			84
#define				Parameter85			85
#define				Parameter86			86
#define				Parameter87			87
#define				Parameter88			88
#define				Parameter89			89
#define				Parameter90			90
#define				Parameter91			91
#define				Parameter92			92
#define				Parameter93			93
#define				Parameter94			94
#define				Parameter95			95
#define				Parameter96			96
#define				Parameter97			97
#define				Parameter98			98
#define				Parameter99			99
#define				Parameter100			100
#define				Parameter101			101
#define				Parameter102			102
#define				Parameter103			103
#define				Parameter104			104
#define				Parameter105			105
#define				Parameter106			106
#define				Parameter107			107
#define				Parameter108			108
#define				Parameter109			109
#define				Parameter110			110
#define				Parameter111			111
#define				Parameter112			112
#define				Parameter113			113
#define				Parameter114			114
#define				Parameter115			115
#define				Parameter116			116
#define				Parameter117			117
#define				Parameter118			118
#define				Parameter119			119
#define				Parameter120			120
#define				Parameter121			121
#define				Parameter122			122
#define				Parameter123			123
#define				Parameter124			124
#define				Parameter125			125
#define				Parameter126			126
#define				Parameter127			127
#define				Parameter128			128
#define				Parameter129			129
#define				Parameter130			130
#define				Parameter131			131
#define				Parameter132			132
#define				Parameter133			133
#define				Parameter134			134
#define				Parameter135			135
#define				Parameter136			136
#define				Parameter137			137
#define				Parameter138			138
#define				Parameter139			139
#define				Parameter140			140
#define				Parameter141			141
#define				Parameter142			142
#define				Parameter143			143
#define				Parameter144			144
#define				Parameter145			145
#define				Parameter146			146
#define				Parameter147			147
#define				Parameter148			148
#define				Parameter149			149
#define				Parameter150			150
#define				Parameter151			151
#define				Parameter152			152
#define				Parameter153			153
#define				Parameter154			154
#define				Parameter155			155
#define				Parameter156			156
#define				Parameter157			157
#define				Parameter158			158
#define				Parameter159			159
#define				Parameter160			160
#define				Parameter161			161
#define				Parameter162			162
#define				Parameter163			163
#define				Parameter164			164
#define				Parameter165			165
#define				Parameter166			166
#define				Parameter167			167
#define				Parameter168			168
#define				Parameter169			169
#define				Parameter170			170
#define				Parameter171			171
#define				Parameter172			172
#define				Parameter173			173
#define				Parameter174			174
#define				Parameter175			175
#define				Parameter176			176
#define				Parameter177			177
#define				Parameter178			178
#define				Parameter179			179
#define				Parameter180			180
#define				Parameter181			181
#define				Parameter182			182
#define				Parameter183			183
#define				Parameter184			184
#define				Parameter185			185
#define				Parameter186			186
#define				Parameter187			187
#define				Parameter188			188
#define				Parameter189			189
#define				Parameter190			190
#define				Parameter191			191
#define				Parameter192			192
#define				Parameter193			193
#define				Parameter194			194
#define				Parameter195			195
#define				Parameter196			196
#define				Parameter197			197
#define				Parameter198			198
#define				Parameter199			199
#define				Parameter200			200
#define				Parameter201			201
#define				Parameter202			202
#define				Parameter203			203
#define				Parameter204			204
#define				Parameter205			205
#define				Parameter206			206
#define				Parameter207			207
#define				Parameter208			208
#define				Parameter209			209
#define				Parameter210			210
#define				Parameter211			211
#define				Parameter212			212
#define				Parameter213			213
#define				Parameter214			214
#define				Parameter215			215
#define				Parameter216			216
#define				Parameter217			217
#define				Parameter218			218
#define				Parameter219			219
#define				Parameter220			220
#define				Parameter221			221
#define				Parameter222			222
#define				Parameter223			223
#define				Parameter224			224
#define				Parameter225			225
#define				Parameter226			226
#define				Parameter227			227
#define				Parameter228			228
#define				Parameter229			229
#define				Parameter230			230
#define				Parameter231			231
#define				Parameter232			232
#define				Parameter233			233
#define				Parameter234			234
#define				Parameter235			235
#define				Parameter236			236
#define				Parameter237			237
#define				Parameter238			238
#define				Parameter239			239
#define				Parameter240			240
#define				Parameter241			241
#define				Parameter242			242
#define				Parameter243			243
#define				Parameter244			244
#define				Parameter245			245
#define				Parameter246			246
#define				Parameter247			247
#define				Parameter248			248
#define				Parameter249			249
#define				Parameter250			250
#define				Parameter251			251
#define				Parameter252			252
#define				Parameter253			253
#define				Parameter254			254
#define				Parameter255			255
#define				Parameter256			256
#define				Parameter257			257
#define				Parameter258			258
#define				Parameter259			259
#define				Parameter260			260
#define				Parameter261			261
#define				Parameter262			262
#define				Parameter263			263
#define				Parameter264			264
#define				Parameter265			265
#define				Parameter266			266
#define				Parameter267			267
#define				Parameter268			268
#define				Parameter269			269
#define				Parameter270			270
#define				Parameter271			271
#define				Parameter272			272
#define				Parameter273			273
#define				Parameter274			274
#define				Parameter275			275
#define				Parameter276			276
#define				Parameter277			277
#define				Parameter278			278
#define				Parameter279			279
#define				Parameter280			280
#define				Parameter281			281
#define				Parameter282			282
#define				Parameter283			283
#define				Parameter284			284
#define				Parameter285			285
#define				Parameter286			286
#define				Parameter287			287
#define				Parameter288			288
#define				Parameter289			289
#define				Parameter290			290
#define				Parameter291			291
#define				Parameter292			292
#define				Parameter293			293
#define				Parameter294			294
#define				Parameter295			295
#define				Parameter296			296
#define				Parameter297			297
#define				Parameter298			298
#define				Parameter299			299
#define				Parameter300			300
#define				Parameter301			301
#define				Parameter302			302
#define				Parameter303			303
#define				Parameter304			304
#define				Parameter305			305
#define				Parameter306			306
#define				Parameter307			307
#define				Parameter308			308
#define				Parameter309			309
#define				Parameter310			310
#define				Parameter311			311
#define				Parameter312			312
#define				Parameter313			313
#define				Parameter314			314
#define				Parameter315			315
#define				Parameter316			316
#define				Parameter317			317
#define				Parameter318			318
#define				Parameter319			319
#define				Parameter320			320
#define				Parameter321			321
#define				Parameter322			322
#define				Parameter323			323
#define				Parameter324			324
#define				Parameter325			325
#define				Parameter326			326
#define				Parameter327			327
#define				Parameter328			328
#define				Parameter329			329
#define				Parameter330			330
#define				Parameter331			331
#define				Parameter332			332
#define				Parameter333			333
#define				Parameter334			334
#define				Parameter335			335
#define				Parameter336			336
#define				Parameter337			337
#define				Parameter338			338
#define				Parameter339			339
#define				Parameter340			340
#define				Parameter341			341
#define				Parameter342			342
#define				Parameter343			343
#define				Parameter344			344
#define				Parameter345			345
#define				Parameter346			346
#define				Parameter347			347
#define				Parameter348			348
#define				Parameter349			349
#define				Parameter350			350
#define				Parameter351			351
#define				Parameter352			352
#define				Parameter353			353
#define				Parameter354			354
#define				Parameter355			355
#define				Parameter356			356
#define				Parameter357			357
#define				Parameter358			358
#define				Parameter359			359
#define				Parameter360			360
#define				Parameter361			361
#define				Parameter362			362
#define				Parameter363			363
#define				Parameter364			364
#define				Parameter365			365
#define				Parameter366			366
#define				Parameter367			367
#define				Parameter368			368
#define				Parameter369			369
#define				Parameter370			370
#define				Parameter371			371
#define				Parameter372			372
#define				Parameter373			373
#define				Parameter374			374
#define				Parameter375			375
#define				Parameter376			376
#define				Parameter377			377
#define				Parameter378			378
#define				Parameter379			379
#define				Parameter380			380
#define				Parameter381			381
#define				Parameter382			382
#define				Parameter383			383
#define				Parameter384			384
#define				Parameter385			385
#define				Parameter386			386
#define				Parameter387			387
#define				Parameter388			388
#define				Parameter389			389
#define				Parameter390			390
#define				Parameter391			391
#define				Parameter392			392
#define				Parameter393			393
#define				Parameter394			394
#define				Parameter395			395
#define				Parameter396			396
#define				Parameter397			397
#define				Parameter398			398
#define				Parameter399			399
#define				Parameter400			400
#define				Parameter401			401
#define				Parameter402			402
#define				Parameter403			403
#define				Parameter404			404
#define				Parameter405			405
#define				Parameter406			406
#define				Parameter407			407
#define				Parameter408			408
#define				Parameter409			409
#define				Parameter410			410
#define				Parameter411			411
#define				Parameter412			412
#define				Parameter413			413
#define				Parameter414			414
#define				Parameter415			415
#define				Parameter416			416
#define				Parameter417			417
#define				Parameter418			418
#define				Parameter419			419
#define				Parameter420			420
#define				Parameter421			421
#define				Parameter422			422
#define				Parameter423			423
#define				Parameter424			424
#define				Parameter425			425
#define				Parameter426			426
#define				Parameter427			427
#define				Parameter428			428
#define				Parameter429			429
#define				Parameter430			430
#define				Parameter431			431
#define				Parameter432			432
#define				Parameter433			433
#define				Parameter434			434
#define				Parameter435			435
#define				Parameter436			436
#define				Parameter437			437
#define				Parameter438			438
#define				Parameter439			439
#define				Parameter440			440
#define				Parameter441			441
#define				Parameter442			442
#define				Parameter443			443
#define				Parameter444			444
#define				Parameter445			445
#define				Parameter446			446
#define				Parameter447			447
#define				Parameter448			448
#define				Parameter449			449
#define				Parameter450			450
#define				Parameter451			451
#define				Parameter452			452
#define				Parameter453			453
#define				Parameter454			454
#define				Parameter455			455
#define				Parameter456			456
#define				Parameter457			457
#define				Parameter458			458
#define				Parameter459			459
#define				Parameter460			460
#define				Parameter461			461
#define				Parameter462			462
#define				Parameter463			463
#define				Parameter464			464
#define				Parameter465			465
#define				Parameter466			466
#define				Parameter467			467
#define				Parameter468			468
#define				Parameter469			469
#define				Parameter470			470
#define				Parameter471			471
#define				Parameter472			472
#define				Parameter473			473
#define				Parameter474			474
#define				Parameter475			475
#define				Parameter476			476
#define				Parameter477			477
#define				Parameter478			478
#define				Parameter479			479
#define				Parameter480			480
#define				Parameter481			481
#define				Parameter482			482
#define				Parameter483			483
#define				Parameter484			484
#define				Parameter485			485
#define				Parameter486			486
#define				Parameter487			487
#define				Parameter488			488
#define				Parameter489			489
#define				Parameter490			490
#define				Parameter491			491
#define				Parameter492			492
#define				Parameter493			493
#define				Parameter494			494
#define				Parameter495			495
#define				Parameter496			496
#define				Parameter497			497
#define				Parameter498			498
#define				Parameter499			499
#define				Parameter500			500
#define				Parameter501			501
#define				Parameter502			502
#define				Parameter503			503
#define				Parameter504			504
#define				Parameter505			505
#define				Parameter506			506
#define				Parameter507			507
#define				Parameter508			508
#define				Parameter509			509
#define				Parameter510			510
#define				Parameter511			511
#define				Parameter512			512
#define				Parameter513			513
#define				Parameter514			514
#define				Parameter515			515
#define				Parameter516			516
#define				Parameter517			517
#define				Parameter518			518
#define				Parameter519			519
#define				Parameter520			520
#define				Parameter521			521
#define				Parameter522			522
#define				Parameter523			523
#define				Parameter524			524
#define				Parameter525			525
#define				Parameter526			526
#define				Parameter527			527
#define				Parameter528			528
#define				Parameter529			529
#define				Parameter530			530
#define				Parameter531			531
#define				Parameter532			532
#define				Parameter533			533
#define				Parameter534			534
#define				Parameter535			535
#define				Parameter536			536
#define				Parameter537			537
#define				Parameter538			538
#define				Parameter539			539
#define				Parameter540			540
#define				Parameter541			541
#define				Parameter542			542
#define				Parameter543			543
#define				Parameter544			544
#define				Parameter545			545
#define				Parameter546			546
#define				Parameter547			547
#define				Parameter548			548
#define				Parameter549			549
#define				Parameter550			550
#define				Parameter551			551
#define				Parameter552			552
#define				Parameter553			553
#define				Parameter554			554
#define				Parameter555			555
#define				Parameter556			556
#define				Parameter557			557
#define				Parameter558			558
#define				Parameter559			559
#define				Parameter560			560
#define				Parameter561			561
#define				Parameter562			562
#define				Parameter563			563
#define				Parameter564			564
#define				Parameter565			565
#define				Parameter566			566
#define				Parameter567			567
#define				Parameter568			568
#define				Parameter569			569
#define				Parameter570			570
#define				Parameter571			571
#define				Parameter572			572
#define				Parameter573			573
#define				Parameter574			574
#define				Parameter575			575
#define				Parameter576			576
#define				Parameter577			577
#define				Parameter578			578
#define				Parameter579			579
#define				Parameter580			580
#define				Parameter581			581
#define				Parameter582			582
#define				Parameter583			583
#define				Parameter584			584
#define				Parameter585			585
#define				Parameter586			586
#define				Parameter587			587
#define				Parameter588			588
#define				Parameter589			589
#define				Parameter590			590
#define				Parameter591			591
#define				Parameter592			592
#define				Parameter593			593
#define				Parameter594			594
#define				Parameter595			595
#define				Parameter596			596
#define				Parameter597			597
#define				Parameter598			598
#define				Parameter599			599
#define				Parameter600			600
#define				Parameter601			601
#define				Parameter602			602
#define				Parameter603			603
#define				Parameter604			604
#define				Parameter605			605
#define				Parameter606			606
#define				Parameter607			607
#define				Parameter608			608
#define				Parameter609			609
#define				Parameter610			610
#define				Parameter611			611
#define				Parameter612			612
#define				Parameter613			613
#define				Parameter614			614
#define				Parameter615			615
#define				Parameter616			616
#define				Parameter617			617
#define				Parameter618			618
#define				Parameter619			619
#define				Parameter620			620
#define				Parameter621			621
#define				Parameter622			622
#define				Parameter623			623
#define				Parameter624			624
#define				Parameter625			625
#define				Parameter626			626
#define				Parameter627			627
#define				Parameter628			628
#define				Parameter629			629
#define				Parameter630			630
#define				Parameter631			631
#define				Parameter632			632
#define				Parameter633			633
#define				Parameter634			634
#define				Parameter635			635
#define				Parameter636			636
#define				Parameter637			637
#define				Parameter638			638
#define				Parameter639			639
#define				Parameter640			640
#define				Parameter641			641
#define				Parameter642			642
#define				Parameter643			643
#define				Parameter644			644
#define				Parameter645			645
#define				Parameter646			646
#define				Parameter647			647
#define				Parameter648			648
#define				Parameter649			649
#define				Parameter650			650
#define				Parameter651			651
#define				Parameter652			652
#define				Parameter653			653
#define				Parameter654			654
#define				Parameter655			655
#define				Parameter656			656
#define				Parameter657			657
#define				Parameter658			658
#define				Parameter659			659
#define				Parameter660			660
#define				Parameter661			661
#define				Parameter662			662
#define				Parameter663			663
#define				Parameter664			664
#define				Parameter665			665
#define				Parameter666			666
#define				Parameter667			667
#define				Parameter668			668
#define				Parameter669			669
#define				Parameter670			670
#define				Parameter671			671
#define				Parameter672			672
#define				Parameter673			673
#define				Parameter674			674
#define				Parameter675			675
#define				Parameter676			676
#define				Parameter677			677
#define				Parameter678			678
#define				Parameter679			679
#define				Parameter680			680
#define				Parameter681			681
#define				Parameter682			682
#define				Parameter683			683
#define				Parameter684			684
#define				Parameter685			685
#define				Parameter686			686
#define				Parameter687			687
#define				Parameter688			688
#define				Parameter689			689
#define				Parameter690			690
#define				Parameter691			691
#define				Parameter692			692
#define				Parameter693			693
#define				Parameter694			694
#define				Parameter695			695
#define				Parameter696			696
#define				Parameter697			697
#define				Parameter698			698
#define				Parameter699			699
#define				Parameter700			700
#define				Parameter701			701
#define				Parameter702			702
#define				Parameter703			703
#define				Parameter704			704
#define				Parameter705			705
#define				Parameter706			706
#define				Parameter707			707
#define				Parameter708			708
#define				Parameter709			709
#define				Parameter710			710
#define				Parameter711			711
#define				Parameter712			712
#define				Parameter713			713
#define				Parameter714			714
#define				Parameter715			715
#define				Parameter716			716
#define				Parameter717			717
#define				Parameter718			718
#define				Parameter719			719
#define				Parameter720			720
#define				Parameter721			721
#define				Parameter722			722
#define				Parameter723			723
#define				Parameter724			724
#define				Parameter725			725
#define				Parameter726			726
#define				Parameter727			727
#define				Parameter728			728
#define				Parameter729			729
#define				Parameter730			730
#define				Parameter731			731
#define				Parameter732			732
#define				Parameter733			733
#define				Parameter734			734
#define				Parameter735			735
#define				Parameter736			736
#define				Parameter737			737
#define				Parameter738			738
#define				Parameter739			739
#define				Parameter740			740
#define				Parameter741			741
#define				Parameter742			742
#define				Parameter743			743
#define				Parameter744			744
#define				Parameter745			745
#define				Parameter746			746
#define				Parameter747			747
#define				Parameter748			748
#define				Parameter749			749
#define				Parameter750			750
#define				Parameter751			751
#define				Parameter752			752
#define				Parameter753			753
#define				Parameter754			754
#define				Parameter755			755
#define				Parameter756			756
#define				Parameter757			757
#define				Parameter758			758
#define				Parameter759			759
#define				Parameter760			760
#define				Parameter761			761
#define				Parameter762			762
#define				Parameter763			763
#define				Parameter764			764
#define				Parameter765			765
#define				Parameter766			766
#define				Parameter767			767
#define				Parameter768			768
#define				Parameter769			769
#define				Parameter770			770
#define				Parameter771			771
#define				Parameter772			772
#define				Parameter773			773
#define				Parameter774			774
#define				Parameter775			775
#define				Parameter776			776
#define				Parameter777			777
#define				Parameter778			778
#define				Parameter779			779
#define				Parameter780			780
#define				Parameter781			781
#define				Parameter782			782
#define				Parameter783			783
#define				Parameter784			784
#define				Parameter785			785
#define				Parameter786			786
#define				Parameter787			787
#define				Parameter788			788
#define				Parameter789			789
#define				Parameter790			790
#define				Parameter791			791
#define				Parameter792			792
#define				Parameter793			793
#define				Parameter794			794
#define				Parameter795			795
#define				Parameter796			796
#define				Parameter797			797
#define				Parameter798			798
#define				Parameter799			799
#define				Parameter800			800
#define				Parameter801			801
#define				Parameter802			802
#define				Parameter803			803
#define				Parameter804			804
#define				Parameter805			805
#define				Parameter806			806
#define				Parameter807			807
#define				Parameter808			808
#define				Parameter809			809
#define				Parameter810			810
#define				Parameter811			811
#define				Parameter812			812
#define				Parameter813			813
#define				Parameter814			814
#define				Parameter815			815
#define				Parameter816			816
#define				Parameter817			817
#define				Parameter818			818
#define				Parameter819			819
#define				Parameter820			820
#define				Parameter821			821
#define				Parameter822			822
#define				Parameter823			823
#define				Parameter824			824
#define				Parameter825			825
#define				Parameter826			826
#define				Parameter827			827
#define				Parameter828			828
#define				Parameter829			829
#define				Parameter830			830
#define				Parameter831			831
#define				Parameter832			832
#define				Parameter833			833
#define				Parameter834			834
#define				Parameter835			835
#define				Parameter836			836
#define				Parameter837			837
#define				Parameter838			838
#define				Parameter839			839
#define				Parameter840			840
#define				Parameter841			841
#define				Parameter842			842
#define				Parameter843			843
#define				Parameter844			844
#define				Parameter845			845
#define				Parameter846			846
#define				Parameter847			847
#define				Parameter848			848
#define				Parameter849			849
#define				Parameter850			850
#define				Parameter851			851
#define				Parameter852			852
#define				Parameter853			853
#define				Parameter854			854
#define				Parameter855			855
#define				Parameter856			856
#define				Parameter857			857
#define				Parameter858			858
#define				Parameter859			859
#define				Parameter860			860
#define				Parameter861			861
#define				Parameter862			862
#define				Parameter863			863
#define				Parameter864			864
#define				Parameter865			865
#define				Parameter866			866
#define				Parameter867			867
#define				Parameter868			868
#define				Parameter869			869
#define				Parameter870			870
#define				Parameter871			871
#define				Parameter872			872
#define				Parameter873			873
#define				Parameter874			874
#define				Parameter875			875
#define				Parameter876			876
#define				Parameter877			877
#define				Parameter878			878
#define				Parameter879			879
#define				Parameter880			880
#define				Parameter881			881
#define				Parameter882			882
#define				Parameter883			883
#define				Parameter884			884
#define				Parameter885			885
#define				Parameter886			886
#define				Parameter887			887
#define				Parameter888			888
#define				Parameter889			889
#define				Parameter890			890
#define				Parameter891			891
#define				Parameter892			892
#define				Parameter893			893
#define				Parameter894			894
#define				Parameter895			895
#define				Parameter896			896
#define				Parameter897			897
#define				Parameter898			898
#define				Parameter899			899
#endif /* FRAME_H_ */
