//C++ Server Prototype
//Note*: Using boost library

#include <iostream>
#include <list>
#include <map>    ?? chi nhi?
#include <queue>
#include <cstdlib>

//Include boost libraries
#include <boost/asio.hpp>
#include <boost/thead.hpp>
#include <boost/asio/ip/tcp.hpp>

//Using namespace cho gon code
using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

//Typedef
typedef boost::shared_ptr<tcp::socket> socket_ptr;
typedef boost::shared_ptr<string> string__ptr;
typedef boost::shared_ptr < queue<string_ptr> > messageQueue_ptr;

//Global variables
io_service service;   //Asynchoronous I/O
messageQueeu_ptr messageQueue(new queue<string_ptr>); //Buffer chung cho consumer-producer
tcp::endpoint ep(ip::address::from_string("127.0.0.1"),50001);
const int inputSize = 256;
string_ptr promptCpy;   //Dong hien thi chat cho user

//Function Prototypes
bool isOwnMessage(string_ptr);
void displayLoop(socket_ptr);
void inboundLoop(socket_ptr, string_ptr);
void writeLoop(socket_ptr, string_ptr);
string* buildPrompt();

//-----Main-----
int main(int argc, char** argv)
{
  try
  {
    boost::thread_groupd threads;
    socket_ptr sock(new tcp::socket(service));
    
    string_ptr prompt(buildPrompt() );
    promptCpy = prompt;
    
    sock->connect(ep);
    
    cout << "Welcome to my Chat Client\nType 'exit' to quit\n\n";
    
    threads.create_thread(boost::bind(displayLoop, sock));
    threads.create_thread(boost::bind(inboundLoop, sock, prompt);
    threads.create_thread(boost::bind(writeLoop, sock, prompt);
    
    threads.join_all();
  }
  catch(std:exception& e)
  {
    cerr << e.what() << endl;
  }
  
  puts("Press any key to continue");
  getc(stdin);
  rerturn EXIT_SUCCESS;
}

//Xuat text ra man hinh
string* buildPrompt()
{
  const int inputSize = 256;
  char inputBuf[inputSize] = {0};
  char nameBuf[inputSize] = {0};
  string* prompt = new string(": ");
  
  cout << "Input your user name: ";
  cin.getline(nameBuf,inputSize);
  *prompt = (string)nameBuf + *prompt;
  boost::algorithm::to_lower(*prompt);
  
  return prompt;
}

void inboundLoop(socket_ptr sock, string_ptr prompt)
{
	int bytesRead = 0;
	char readBuf[1024] = {0};

	for(;;)
	{
		if(sock->available())
		{
			bytesRead = sock->read_some(buffer(readBuf, inputSize));
			string_ptr msg(new string(readBuf, bytesRead));

			messageQueue->push(msg);
		}

		boost::this_thread::sleep( boost::posix_time::millisec(1000));
	}
}

void writeLoop(socket_ptr sock, string_ptr prompt)
{
	char inputBuf[inputSize] = {0};
	string inputMsg;

	for(;;)
	{
		cin.getline(inputBuf, inputSize);
		inputMsg = *prompt + (string)inputBuf + '\n';

		if(!inputMsg.empty())
		{
			sock->write_some(buffer(inputMsg, inputSize));
		}

		if(inputMsg.find("exit") != string::npos)
			exit(1);

		inputMsg.clear();
		memset(inputBuf, 0, inputSize);
	}
}

void displayLoop(socket_ptr sock)
{
	for(;;)
	{
		if(!messageQueue->empty())
		{
			if(!isOwnMessage(messageQueue->front()))
			{
				cout << "\n" + *(messageQueue->front());
			}

			messageQueue->pop();
		}

		boost::this_thread::sleep( boost::posix_time::millisec(1000));
	}
}

bool isOwnMessage(string_ptr message)
{
	if(message->find(*promptCpy) != string::npos)
		return true;
	else
		return false;
}
