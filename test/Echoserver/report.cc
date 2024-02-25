root@hcss-ecs-7fa1:~/miniWebServer/test-build# valgrind --leak-check=yes ./echoserver 
==22198== Memcheck, a memory error detector
==22198== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==22198== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==22198== Command: ./echoserver
==22198== 
Message from client GET / HTTP/1.1
Host: yourhostname.com


==22198== Thread 2:
==22198== Invalid read of size 4
==22198==    at 0x489FB0A: Channel::handleEvent(Timestamp&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A5C36: EventLoop::loop() (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A6B95: Server::Start()::{lambda()#1}::operator()() const (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A9A62: void std::__invoke_impl<void, Server::Start()::{lambda()#1}&>(std::__invoke_other, Server::Start()::{lambda()#1}&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A95C3: std::__invoke_result<Server::Start()::{lambda()#1}&>::type std::__invoke<Server::Start()::{lambda()#1}&>(Server::Start()::{lambda()#1}&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A9369: void std::_Bind<Server::Start()::{lambda()#1} ()>::__call<void>(std::tuple<>&&, std::_Index_tuple<>) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A8F78: void std::_Bind<Server::Start()::{lambda()#1} ()>::operator()<, void>() (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A8C8F: void std::__invoke_impl<void, std::_Bind<Server::Start()::{lambda()#1} ()>&>(std::__invoke_other, std::_Bind<Server::Start()::{lambda()#1} ()>&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A89F6: std::enable_if<is_invocable_r_v<void, std::_Bind<Server::Start()::{lambda()#1} ()>&>, void>::type std::__invoke_r<void, std::_Bind<Server::Start()::{lambda()#1} ()>&>(std::_Bind<Server::Start()::{lambda()#1} ()>&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A875C: std::__future_base::_Task_state<std::_Bind<Server::Start()::{lambda()#1} ()>, std::allocator<int>, void ()>::_M_run()::{lambda()#1}::operator()() const (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A95F7: std::__future_base::_Task_setter<std::unique_ptr<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter>, std::__future_base::_Task_state<std::_Bind<Server::Start()::{lambda()#1} ()>, std::allocator<int>, void ()>::_M_run()::{lambda()#1}, void>::operator()() const (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A93E0: std::unique_ptr<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter> std::__invoke_impl<std::unique_ptr<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter>, std::__future_base::_Task_setter<std::unique_ptr<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter>, std::__future_base::_Task_state<std::_Bind<Server::Start()::{lambda()#1} ()>, std::allocator<int>, void ()>::_M_run()::{lambda()#1}, void>&>(std::__invoke_other, std::__future_base::_Task_setter<std::unique_ptr<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter>, std::__future_base::_Task_state<std::_Bind<Server::Start()::{lambda()#1} ()>, std::allocator<int>, void ()>::_M_run()::{lambda()#1}, void>&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==  Address 0x4e85a94 is 20 bytes inside a block of size 152 free'd
==22198==    at 0x484BB6F: operator delete(void*, unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22198==    by 0x48A1AD2: std::default_delete<Channel>::operator()(Channel*) const (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A1217: std::unique_ptr<Channel, std::default_delete<Channel> >::~unique_ptr() (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A0299: Connector::~Connector() (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48B441F: std::_Sp_counted_ptr<Connector*, (__gnu_cxx::_Lock_policy)2>::_M_dispose() (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A211D: std::_Sp_counted_base<(__gnu_cxx::_Lock_policy)2>::_M_release() (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A1712: std::__shared_count<(__gnu_cxx::_Lock_policy)2>::~__shared_count() (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A0F83: std::__shared_ptr<Connector, (__gnu_cxx::_Lock_policy)2>::~__shared_ptr() (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A0FA3: std::shared_ptr<Connector>::~shared_ptr() (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48B1DA7: std::pair<int const, std::shared_ptr<Connector> >::~pair() (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48B1DC6: void std::destroy_at<std::pair<int const, std::shared_ptr<Connector> > >(std::pair<int const, std::shared_ptr<Connector> >*) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48B0C55: void std::allocator_traits<std::allocator<std::__detail::_Hash_node<std::pair<int const, std::shared_ptr<Connector> >, false> > >::destroy<std::pair<int const, std::shared_ptr<Connector> > >(std::allocator<std::__detail::_Hash_node<std::pair<int const, std::shared_ptr<Connector> >, false> >&, std::pair<int const, std::shared_ptr<Connector> >*) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==  Block was alloc'd at
==22198==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22198==    by 0x48A1171: std::_MakeUniq<Channel>::__single_object std::make_unique<Channel, int&, EventLoop*&>(int&, EventLoop*&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x489FE5E: Connector::Connector(int, EventLoop*, InetAddress const&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48A696C: Server::createConnection(int, InetAddress const&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48B33A9: void std::__invoke_impl<void, void (Server::*&)(int, InetAddress const&), Server*&, int, InetAddress const&>(std::__invoke_memfun_deref, void (Server::*&)(int, InetAddress const&), Server*&, int&&, InetAddress const&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48B28EA: std::__invoke_result<void (Server::*&)(int, InetAddress const&), Server*&, int, InetAddress const&>::type std::__invoke<void (Server::*&)(int, InetAddress const&), Server*&, int, InetAddress const&>(void (Server::*&)(int, InetAddress const&), Server*&, int&&, InetAddress const&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48B1F22: void std::_Bind<void (Server::*(Server*, std::_Placeholder<1>, std::_Placeholder<2>))(int, InetAddress const&)>::__call<void, int&&, InetAddress const&, 0ul, 1ul, 2ul>(std::tuple<int&&, InetAddress const&>&&, std::_Index_tuple<0ul, 1ul, 2ul>) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48B0E0A: void std::_Bind<void (Server::*(Server*, std::_Placeholder<1>, std::_Placeholder<2>))(int, InetAddress const&)>::operator()<int, InetAddress const&, void>(int&&, InetAddress const&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48AFA36: void std::__invoke_impl<void, std::_Bind<void (Server::*(Server*, std::_Placeholder<1>, std::_Placeholder<2>))(int, InetAddress const&)>&, int, InetAddress const&>(std::__invoke_other, std::_Bind<void (Server::*(Server*, std::_Placeholder<1>, std::_Placeholder<2>))(int, InetAddress const&)>&, int&&, InetAddress const&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48AE290: std::enable_if<is_invocable_r_v<void, std::_Bind<void (Server::*(Server*, std::_Placeholder<1>, std::_Placeholder<2>))(int, InetAddress const&)>&, int, InetAddress const&>, void>::type std::__invoke_r<void, std::_Bind<void (Server::*(Server*, std::_Placeholder<1>, std::_Placeholder<2>))(int, InetAddress const&)>&, int, InetAddress const&>(std::_Bind<void (Server::*(Server*, std::_Placeholder<1>, std::_Placeholder<2>))(int, InetAddress const&)>&, int&&, InetAddress const&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x48AD064: std::_Function_handler<void (int, InetAddress const&), std::_Bind<void (Server::*(Server*, std::_Placeholder<1>, std::_Placeholder<2>))(int, InetAddress const&)> >::_M_invoke(std::_Any_data const&, int&&, InetAddress const&) (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198==    by 0x489E12F: std::function<void (int, InetAddress const&)>::operator()(int, InetAddress const&) const (in /root/miniWebServer/test-build/src/libmyserver.so)
==22198== 
