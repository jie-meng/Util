FROM gcc

WORKDIR /app

RUN apt-get update &&\
        apt-get -y install cmake &&\
        apt-get -y install python3

CMD ["sh"]
