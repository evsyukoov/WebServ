//
// Created by Денис on 13/12/2020.
//


int 	main()
{
	Net net("127.0.0.1:8000");
	int fd = net.connect();
	std::cout << "Connection established! fd: " << fd << std::endl;
	if (fd < 0)
		return (-1);
	char buff[32];
	std::cin >> buff;
	buff[31] = '\0';
	//отправляем на сервер
	net.send(fd, buff, 32);
	//получаем с сервера
	net.recv(fd, buff, 32);

	std::cout << buff << std::endl;
	net.close(fd);
}
