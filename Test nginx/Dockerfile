FROM ubuntu:latest

COPY ./Server ./Server


RUN apt update 

EXPOSE 80 1234 3456 

#RUN yum update && \
# yum install -y make && \
# yum install -y clang++

RUN apt-get install cmake clang bison flex xz-utils libfuse-dev \ 
 libudev-dev pkg-config libc6-dev:i386 linux-headers-generic \ 
 gcc-multilib libcap2-bin libcairo2-dev libgl1-mesa-dev libtiff5-dev \ 
 libfreetype6-dev libfreetype6-dev:i386 git libelf-dev libxml2-dev \ 
 libegl1-mesa-dev libfontconfig1-dev libbsd-dev

RUN apt install git

RUN apt install cmake

RUN git clone --recursive https://github.com/darlinghq/darling.git

RUN mkdir -C ./build && cd buildx && cmake ..

RUN make

#UN make -C ./Server

CMD [ "bash" ]
#CMD ["./Server/server", "./Server/CGI_Scrypts/cgi_tester", "./Server/Config.conf"]
