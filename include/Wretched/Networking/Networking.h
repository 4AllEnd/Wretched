#pragma once
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/TCPServer.h>
#include <vector>
#include <iostream>
#include <string>
typedef unsigned char byte;
namespace Wretched{
	struct Packet{
		Poco::Net::SocketAddress sender;
		std::vector<byte>data;
		Packet(Poco::Net::SocketAddress, byte* bytes,unsigned int size){ 
			data.resize(size);
			data.insert(data.begin(),bytes,bytes+size); 
			};
		};
	///acts as login server, "mail" server, and streaming server
	struct TCPServer{
		Poco::Net::ServerSocket socket;
		void listen(){
			socket.listen();
			Poco::Net::SocketAddress caddy;
			Poco::Net::StreamSocket a=socket.acceptConnection(caddy);
			byte h[65500];
			Packet(caddy, &h[0],(unsigned int) sizeof(h));
			std::string s=(char*)h;
			if (h[0] != 0){ std::cout << s.c_str() << "\n" << s.size() << "\n" << socket.address().toString() << "\t" << caddy.toString() << std::endl; }
			}
		void setaddress(std::string a){ socket.bind(Poco::Net::SocketAddress(a)); }
		};
	struct UDPServer{
		Poco::Net::DatagramSocket socket;
		public:
			void listen(){
				Poco::Net::SocketAddress caddy;
				char h[5000];
				socket.receiveFrom((void*)h, sizeof(h), caddy, 0);
				std::string s(h);
				if (h[0] != 0){ std::cout << s.c_str() << "\n" << s.size() << "\n" << socket.address().toString() << "\t" << caddy.toString() << std::endl; }
			}
			void setaddress(std::string a){ socket.bind(Poco::Net::SocketAddress(a)); }
		};
	class Server{	
		UDPServer liveserver;
		TCPServer loginserver, mailserver;
		public:
			void LoginListen(){
			
				};
			void StreamingListen(){
				liveserver.listen();
				}
			void StreamingBroadcast(){}
			void setliveaddress(std::string a){ liveserver.setaddress(a);}
			void setmailaddress(std::string a){ mailserver.setaddress(a);}
			void setloginaddress(std::string a){ loginserver.setaddress(a);}
			void StreamingSend(){}
		};
	class Client{
	public:
		Client(){};
		Poco::Net::DatagramSocket udpsocket;
		Poco::Net::StreamSocket tcpsocket;
		bool connect(std::string a){
			udpsocket.connect(Poco::Net::SocketAddress(a));
			return true;
			}

		};

	}