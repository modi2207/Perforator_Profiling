
## CPU Burner Application Profiling 

We profiled a C++ application containing a function named inefficient_calc_sum(), which performs a summation of vector elements using a large for loop. we identified that the inefficient_calc_sum() function consumed a significant amount of CPU resources—approximately 11.4 billion cycles. To optimize the function, we replaced the for loop with a direct mathematical equation that yields equivalent results. The complete source code for this application is available in code directory. 


1. Since the application needs to run inside a pod, we must first create a Docker image for it. Below are the Dockerfiles for both the non-optimized and optimized versions of the code.

    1.1 Without Optimization
        
        ```bash
            FROM ubuntu:22.04
            RUN apt-get update && apt-get install -y clang
            COPY . /app
            # RUN mkdir /app
            WORKDIR /app
            RUN clang++ -g main_without_optimization.cpp -o main_without_optimization
            CMD ["./main_without_optimization"]
        ```

    1.2 With Optimization 
        
        ```bash
            FROM ubuntu:22.04
            RUN apt-get update && apt-get install -y clang
            COPY . /app
            # RUN mkdir /app
            WORKDIR /app
            RUN clang++ -g main_with_optimization.cpp -o main_with_optimization
            CMD ["./main_with_optimization"]
        ```
    
2. Build image and  push on to docker hub(For that, you have to make repository on docker hub first). 

    docker build -t chirag658/main_without_optimization . 
    docket push chirag658/main_without_optimization:latest
    docker build -t chirag658/main_with_optimization .
    docket push chirag658/main_with_optimization:latest

3. Create Deployment file for both without optimized and with optimized  images. 

```bash
apiVersion: apps/v1
kind: Deployment
metadata:
  name: without_app-deployment
spec:
  selector:
    matchLabels:
      app: without_app
  replicas: 1
  template:
    metadata:
      labels:
        app: without_app
    spec:
      containers:
      - name: without_app
        image: chirag658/main_without_optimization:latest
```

4. Deploying the Application to the Perforator Namespace

    ```bash
     kubectl apply -f with_optimization_app_deployment.yaml -n perforator
     kubectl apply -f without_optimization_app_deployment.yaml -n perforator
    ```

5. In the Web UI, you can view the name of the pod running of application.
