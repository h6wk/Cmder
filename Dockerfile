# https://lemariva.com/blog/2020/10/vscode-c-development-and-debugging-containers

FROM debian:bookworm

LABEL description="Container for use with Visual Studio Code" 

# install build dependencies to build and debug 
RUN apt-get update \
    && apt-get install -y g++ build-essential make cmake gdb gdbserver \
       rsync zip openssh-server git 

# install application dependencies
RUN apt-get install -y libopenblas-dev liblapack-dev python3-dev \ 
    libncurses5-dev libncursesw5-dev libasio-dev libboost-all-dev

# install wget
RUN apt-get install -y wget

# configure SSH for communication with Visual Studio Code
RUN mkdir -p /var/run/sshd

RUN echo 'root:root' | chpasswd \
    && sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config \
    && sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

RUN mkdir -p /source

# pull in bazelisk and move into a directory of PATH
RUN wget https://github.com/bazelbuild/bazelisk/releases/download/v1.17.0/bazelisk-linux-amd64
RUN chmod +x bazelisk-linux-amd64
RUN mv bazelisk-linux-amd64 /usr/local/bin/bazel

WORKDIR /source

CMD ["/usr/sbin/sshd", "-D"]

EXPOSE 22