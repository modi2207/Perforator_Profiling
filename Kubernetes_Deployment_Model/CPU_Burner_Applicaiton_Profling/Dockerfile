FROM ubuntu:22.04
RUN apt-get update && apt-get install -y clang
COPY . /app
# RUN mkdir /app
WORKDIR /app
RUN clang++ -g main_without_optimization.cpp -o main_without_optimization
CMD ["./main_without_optimization"]