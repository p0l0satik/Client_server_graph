FROM alpine
RUN apk update
RUN apk upgrade
RUN apk add g++
COPY ./ ./
RUN ./make_server.sh
CMD ./sv
