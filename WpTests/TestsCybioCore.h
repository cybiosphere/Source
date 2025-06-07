#pragma once

#include "CBiotop.h"

extern size_t nbTestsFailed;

#define EXPECT_EQ(val1, val2) if(val1 != val2) {printf("*** ERROR EXPECT_EQ failed line %u found=%d expected=%d\n", __LINE__, val1, val2); nbTestsFailed++;}
#define EXPECT_NE(val1, val2) if(val1 == val2) {printf("*** ERROR EXPECT_NE failed line %u found=%d not expected=%d\n", __LINE__, val1, val2); nbTestsFailed++;}
#define EXPECT_STREQ(val1, val2) if(val1 != val2) {printf("*** ERROR EXPECT_STREQ failed line %u found=%s expected=%s\n", __LINE__, val1, val2); nbTestsFailed++;}
#define EXPECT_LT(val1, val2) if(val1 > val2) {printf("*** ERROR EXPECT_LT failed line %u found=%d should be less than %d\n", __LINE__, val1, val2); nbTestsFailed++;}

class TestsCybioCore
{
public:
  TestsCybioCore(string pathForEntityFiles) : pathForEntityFiles(pathForEntityFiles) {};
  void launchAllTests();

protected:
  void testBiotopCreation();
  void testAnimalBehavior();
  void testAnimalSensors();

private:
  void clearMap(CBiotop* pBiotop);
  size_t countColorizedGrid(CBiotop* pBiotop);
  void verifySensorsRange(CBiotop* pBiotop, CAnimal* pAnimal, 
                          size_t expectGridViewFar, size_t expectGridEar, size_t expectGridSmell, size_t expectGridPhero);
  string pathForEntityFiles;

};

