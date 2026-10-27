FROM ubuntu:latest

RUN apt update \
    && apt-get install -y wget gcc clang cmake libpcre2-dev nano

CMD ["/bin/bash"]
