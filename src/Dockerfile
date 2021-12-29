FROM alpine:3.15

RUN apk update
RUN apk add alpine-sdk gcc cmake

ADD solver.cpp /app/solver.cpp
ADD inputUtility.cpp /app/inputUtility.cpp
ADD inputUtility.hpp /app/inputUtility.hpp
ADD Makefile /app/Makefile

WORKDIR /app/
RUN make