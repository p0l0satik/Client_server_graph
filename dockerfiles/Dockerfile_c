FROM alpine
RUN apk update
RUN apk upgrade
RUN apk add g++
COPY ./ ./
RUN chmod 777 client.cpp
RUN ./make_client.sh
CMD ./cl
