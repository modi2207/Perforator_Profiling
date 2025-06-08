FROM ubuntu:22.04
RUN apt-get update && apt-get install -y clang
COPY . /app
# RUN mkdir /app
WORKDIR /app
RUN clang++ -g server.cpp -o server
CMD ["./server"]
