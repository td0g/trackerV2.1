//Test w/ 8MHZ found 1 us program = ~8 us realworld
//Upload using 1MHZ clock

    PROGMEM const unsigned long delayChangeTable[] = {336,3148,3210,3273,3339,3406,3478,3551,3626,3706,3789,3874,3963,4056,4153,4255,4360,4472,4588,4709,4838,4972,5115,5264,5422,5589,5767,5955,6156,6369,6598,6844,7106,7391,7699,8033,8397,8797,9237,9725,10270,10882,11577,12374,13300,14394,15710,17334,19406,22172,26134,32503,45571,0,214164,57648,44506,38061,34027,31188,29045,27351,25967,24805,23812,22949,22188,21513,20905,20355,19853,19393,18969,18576,18210,17869,17548,17248,16963,16696,16441,16200,15970,15751,15541,15342,15149,14966,14789,14619,14454,14297,14144,13997,13854,13717,13582,13453,13328,13205,13086,12971,12858,12749,12643,12538,12438,12338,12242,12148,12055,11966,11877,11792,11707,11624,11543,11465,11386,11311,11235,11163,11090,11020,10950,10882,10816,10749,10685,10621,10559,10497,10437,10377,10319,10261,10204,10148,10093,10038,9985,9932,9881,9829,9778,9728,9680,9630,9583,9536,9488,9443,9398,9352,9308,9264,9221,9179,9136,9094,9053,9012,8972,8933,8892,8854,8816,8777,8740,8702,8666,8629,8593,8558,8522,8487,8452,8418,8385,8350,8318,8285,8252,8219,8188,8156,8125,8094,8063,8032,8002,7972,7943,7913,7884,7855,7827,7798,7770,7742,7715,7687,7660,7633,7606,7580,7553,7527,7502,7476,7450,7425,7400,7375,7351,7326,7302,7277,7254,7230,7207,7183,7160,7137,7115,7091,7069,7047,7024,7003,6980,6959,6937,6916,6895,6873,6853,6831,6811,6791,6770,6750,6730,6709,6690,6670,6651,6631,6612,6592,6574,6554,6536,6517,6498,6480,6461,6444,6425,6407,6389,6371,6354,6336,6318,6301,6284,6267,6249,6233,6216,6198,6183,6165,6150,6133,6116,6100,6085,6068,6052,6036,6021,6005,5990,5974,5958,5943,5928,5913,5898,5883,5868,5853,5838,5824,5809,5795,5780,5765,5752,5737,5723,5709,5695,5682,5667,5653,5640,5626,5613,5599,5586,5572,5559,5546,5532,5520,5506,5494,5480,5468,5455,5442,5429,5417,5404,5392,5380,5367,5354,5343,5330,5318,5306,5294,5282,5271,5258,5246,5235,5223,5211,5200,5188,5177,5165,5154,5143,5131,5120,5109,5097,5087,5075,5065,5053,5043,5032,5021,5010,5000,4988,4979,4967,4957,4947,4936,4926,4915,4906,4894,4885,4874,4865,4854,4844,4834,4824,4814,4804,4795,4784,4775,4765,4755,4746,4736,4726,4717,4707,4698,4689,4679,4669,4661,4651,4641,4633,4623,4615,4605,4596,4587,4578,4569,4559,4552,4542,4533,4525,4516,4507,4498,4490,4481,4472,4464,4455,4447,4438,4430,4421,4413,4404,4397,4388,4379,4372,4363,4355,4347,4338,4331,4322,4315,4306,4299,4290,4283,4274,4267,4259,4251,4243,4236,4227,4220,4213,4204,4197,4190,4182,4174,4167,4159,4152,4144,4137,4129,4122};   //Massive Table
    const unsigned long totalSteps = 4018969;               //Total steps to end of rod
    int delayChange = 1;                                    //Change in delay time per delay change
    const unsigned int initStepDelay = 3072;            //Starting motor step delay

unsigned int currStepDelay = initStepDelay;           //Starting motor step delay
unsigned int delayChangePhase;
unsigned long nextDelayChangeStep;
unsigned long currStepPosition;
unsigned long nextStepTime;

const byte button = 0;

void setup() {

nextDelayChangeStep = pgm_read_dword_near(delayChangeTable);
  for (byte i = 1; i < 5; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  pinMode(button, INPUT);
  nextStepTime = micros();
  digitalWrite(1, HIGH);
}



void loop() {
  if (micros() > nextStepTime){
    moveMotor();
    updateDelay();
    nextStepTime += currStepDelay;
  }
}

void moveMotor(){
    currStepPosition++;
    switch (currStepPosition % 8){
      case 0:
        digitalWrite(1, HIGH);
      break;
      case 1:
        digitalWrite(4, LOW);
      break;
      case 2:
        digitalWrite(3, HIGH);
      break;
      case 3:
        digitalWrite(1, LOW);
      break;
      case 4:
        digitalWrite(2, HIGH);
      break;
      case 5:
        digitalWrite(3, LOW);
      break;
      case 6:
        digitalWrite(4, HIGH);
      break;
      case 7:
        digitalWrite(2, LOW);
      break;
    }
}



void updateDelay(){
  if (currStepPosition == nextDelayChangeStep){
    currStepDelay -= delayChange;
    delayChangePhase++;
    unsigned int tableValue = pgm_read_dword_near(delayChangeTable + delayChangePhase);
    if (!tableValue) {
      delayChange *= -1;
      delayChangePhase++;
      tableValue = pgm_read_dword_near(delayChangeTable + delayChangePhase);
    }
    nextDelayChangeStep += tableValue;
  }
}
