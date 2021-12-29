FROM alpine-3.15

RUN apk add gcc
RUN apk add make

ADD solver.cpp /app/solver.cpp
ADD inputUtility.cpp /app/inputUtility.cpp
ADD inputUtility.hpp /app/inputUtility.hpp
ADD Makefile /app/Makefile

WORKDIR /app/
CMD make