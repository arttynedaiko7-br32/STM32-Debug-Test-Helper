/*
 * tester.h
 *
 *  Created on: Sep 26, 2025
 *      Author: Artem Daineko
 */

#ifndef INC_TESTER_H_
#define INC_TESTER_H_

//#include <exception
#include <string>
#include <map>
//#include <set>
#include <vector>
//#include <stdexcept>
#include <cstdio>
#include <assert.h>
#include <memory.h>



//# instructions executed = DWT_CYCCNT - DWT_CPICNT - DWT_EXCCNT -
//                            DWT_SLEEPCNT - DWT_LSUCNT + DWT_FOLDCNT


/*
 *
 * int _write(int32_t file, uint8_t *ptr, int32_t len)
 *  {
 *   for (int i = 0; i < len; i++)
 *   {
 *   	HAL_UART_Transmit(&huart1, (uint8_t *)&ptr[i], 1, HAL_MAX_DELAY);
 *   }
 *   return len;
 *  }
 *
 *
 *
 *
 *
 * */

#define CALLBACK_DWT_Init (DWT_Init())

////////////////Compile settings//////////////////////////////////////
//#define SWO 1  // comment or uncomment this is lines
#define UART 1
//////////////////////////////////////////////////////////////////////




class LogMessage{  // we will be push message and store it, after the function finished his work and stream message out.

 LogMessage(const std::string& log,uint8_t& id)
{
	 msg[id].push_back(log);
}


private:
 using Message = std::map<uint8_t,std::vector<std::string>>;
 Message msg;
};




class Profiler {
public:
  explicit Profiler(const std::string& msg = "")
    : message(msg + ": ")
    , start(DWT->CYCCNT)
  {

  }



  ~Profiler() {
// This is reference taken from Arm cortex m3,m4 manual.
// Use it if profiler turn on
//# instructions executed = DWT_CYCCNT - DWT_CPICNT - DWT_EXCCNT -
//                          DWT_SLEEPCNT - DWT_LSUCNT + DWT_FOLDCNT
    int64_t finish = DWT->CYCCNT - DWT->CPICNT - DWT->EXCCNT -
    		         DWT->SLEEPCNT - DWT->LSUCNT+DWT->FOLDCNT;
   // float dur = (float(abs(finish - start))/SystemCoreClock)*1000;
    float dur = (float(abs(finish - start))/HAL_RCC_GetHCLKFreq())*1000;
    sprintf(profiler_message,"%s %f %s \r \n ",message.c_str(),dur,"ms");
    printf(profiler_message);
  }

private:
  char profiler_message[150];
  std::string message;
  int64_t start;

  static bool SWO_UART;


};


[[maybe_unused]] int  DWT_Init(void);

[[maybe_unused]] int  DWT_Deinit(void);

__STATIC_INLINE int UART_SendChar(uint32_t str);


#ifdef SWO

//#ifdef __cplusplus

extern "C"{

 int __io_putchar(uint8_t ch)
 {
 	   return ITM_SendChar(ch);
 }

}

#elif UART

extern "C"{

int __io_putchar(uint8_t ch)
 {
 	   return UART_SendChar(ch);
 }

 }//extern "C"
  //int setUART = [](){return Profiler::setUART();}();

    [[maybe_unused]] int c=[]()
                   constexpr{return DWT_Init();}(); //This is wrapper for callback function DWT_Init

#endif



//#ifdef  USE_FULL_ASSERT 1




 // sprintf - необходим для записи информации о ошибке
 // fprint(stderr,sprintf) - нужен будет для вывода информации в консоль отладки

/*
 * int _write(int32_t file, uint8_t *ptr, int32_t len)
*{
 *   for (int i = 0; i < len; i++)
 *   {
  *      ITM_SendChar(*ptr++);
   * }
   * return len;
*}
*/



/*

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
  os << "{";
  bool first = true;
  for (const auto& kv : m) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}
*/

static int test_count = {0};

enum struct StateTests : uint8_t
{
  OK,
  FAIL,
  UNKNOWN

}sTests = StateTests::OK;


 void bufferClear(char* buffer)
 {
	 buffer[0]= '\0';
	 //memset(buffer,'\0',sizeof(*buffer));

 }

template <class T, class U>
void AssertEqual(const T &t, const U &u,const std::string& hint={})// const char &hint = {}) {
{
	char assert_message[150];

	if (t != u)
	{
	  ++test_count;
	  sTests = StateTests::FAIL;
	 // sprintf(assert_message,"%s %d %s %d %s %d \r\n","Assertion failed:",t,"!=",u,
			//                                         (uint8_t *)__FILE__, __LINE__);
	  sprintf(assert_message,"%s %i %s %i \r \n","Assertion failed:",t,"!=",u);


	  printf(assert_message);
	  bufferClear(assert_message);

      if (!hint.empty())
      {
        sprintf(assert_message,"hint:%s \n",hint.c_str());
	    printf(assert_message);
	    bufferClear(assert_message);
      }
	}

	else if(t == u)
   {
	 (test_count == 0) ? sTests = StateTests::OK
				       : sTests = StateTests::FAIL;
   }
	else
   {
	 ++test_count;
	 sTests = StateTests::UNKNOWN;
   }

}

void Assert(bool b,const std::string& hint) //const char& hint)
{
  AssertEqual(b, true, hint);
}

class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const std::string &test_name) {

	  test_count = 0;
      func();

      switch(sTests)
    {
      case  StateTests::OK:
      {
        sprintf(test_message,"%s %c %s \r \n",test_name.c_str(),'-',"OK!");
    	printf(test_message);
    	bufferClear(test_message);
    	break;
      }
      case  StateTests::FAIL:
      {
    	 ++fail_count;
    	 sprintf(test_message,"%s %c %s \r \n",test_name.c_str(),'-',"Fail!");
    	 printf(test_message);
    	 bufferClear(test_message);
    	 break;
      }
      case  StateTests::UNKNOWN:
      {
    	 ++fail_count;
    	 sprintf(test_message,"%s \r \n","Unknown exception caught.");
    	 printf(test_message);
    	 bufferClear(test_message);
    	 break;
      }
      default: break;

    }

  }


  ~TestRunner() {
    if (fail_count > 0) {
    	sprintf(test_message,"%d %s \r \n",fail_count,"unit tests failed. Terminate.");
    	printf(test_message);
    	bufferClear(test_message);
        exit(1);
    }
      //sprintf(test_message,"%s \r \n","The tests were successful...\n Good job!");
     // printf(test_message);
  }

private:
  int fail_count ={0};
  char message_buff[150];
  char* test_message = &message_buff[0];

};



#define ASSERT_EQUAL(x, y) {                  \
 char assert_info[150];                       \
 sprintf(assert_info,                         \
 "%s %s %s %s %s %s %d \r \n",#x,"!=",#y,",", \
  (uint8_t *)__FILE__ ,":", __LINE__);        \
 AssertEqual(x, y, std::string(assert_info)); \
}

#define ASSERT(x) {                         \
  char assert_info[150];                    \
  sprintf(assert_info,"%s %s %s %s %d \r\n",\
   #x," is false, ",__FILE__,":",__LINE__); \
  Assert(x, std::string(assert_info));      \
}

#define RUN_TEST(tr, func) \
  tr.RunTest(func, #func)


#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)

#define LOG_DURATION(message) \
		Profiler UNIQ_ID(__LINE__){message};


int UART_SendChar(uint32_t str)
{

	//while(*str != 0)
     //{
    	 HAL_UART_Transmit(&huart1, (uint8_t *)&str, 1, 10);
    	 return (str);
     //}

}
 int  DWT_Init(void)
{
   CoreDebug-> DEMCR|= CoreDebug_DEMCR_TRCENA_Msk; // enable debug
   DWT->CYCCNT = DWT->CPICNT= DWT->EXCCNT = DWT->SLEEPCNT = DWT->LSUCNT = DWT->FOLDCNT = {0};
   DWT->CTRL = {0x40000000};//Reset register state
   DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; //Enable CYCCNT
   return  (DWT->CTRL >> 24) & DWT_CTRL_CYCCNTENA_Msk; //(DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) &;
}

 int DWT_Deinit(void)
{
	CoreDebug-> DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // Disable debug
	DWT->CTRL &= (~DWT_CTRL_CYCCNTENA_Msk); //Disable CYCCNT
	return (DWT->CTRL & ~DWT_CTRL_CYCCNTENA_Msk);//  (~DWT_CTRL_CYCCNTENA_Msk);
}



#endif /* INC_TESTER_H_ */
