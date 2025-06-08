
## TCP Server Application

we profiled a simple TCP server application written in C++. This server accepts connections from clients, receives a string, and responds with the reversed version of that string. To profile this application, we containerized the C++ code and deployed it as a pod within the Kubernetes cluster. Our focus was primarily on analyzing the RX path, including both kernel-level and user-space library functions involved in handling incoming data. To simulate traffic, we ran a client-side script outside the cluster that periodically sent requests to the TCP server pod.



1. Since the TCP server needs to run inside a pod, we must first create a Docker image for it. 

        
        ```bash
          FROM ubuntu:22.04
          RUN apt-get update && apt-get install -y clang
          COPY . /app
          # RUN mkdir /app
          WORKDIR /app
          RUN clang++ -g server.cpp -o server
          CMD ["./server"]

        ```
    
2. Build image and  push on to docker hub(For that, you have to make repository on docker hub first). 

    docker build -t chirag658/tcp_server. 
    docket push chirag658/tcp_server:latest

3. Create Deployment file for both without optimized and with optimized  images. 

```bash
apiVersion: apps/v1
kind: Deployment
metadata:
  name: tcp-server
spec:
  replicas: 1
  selector:
    matchLabels:
      app: tcp-server
  template:
    metadata:
      labels:
        app: tcp-server
    spec:
      containers:
      - name: tcp-server
        image: chirag658/tcp_server:latest
        ports:
        - containerPort: 12000
---
apiVersion: v1
kind: Service
metadata:
  name: tcp-server-service
spec:
  selector:
    app: tcp-server
  ports:
    - protocol: TCP
      port: 12000
      targetPort: 12000
  type: LoadBalancer  # or NodePort for local clusters

```

4. Deploying the Application to the Perforator Namespace

    ```bash
     kubectl apply -f tcp_deployment.yaml -n perforator
    ```

5. In the deployment, we use a LoadBalancer service to expose the server, allowing external access through   the load balancer.

    To access the application:

    Use the Minikube IP, which you can get by running:

        ```bash 
            minikube ip
        ```
    Then retrieve the NodePort assigned to the LoadBalancer service using:

        ``` bash 
        kubectl get svc -n perforator
        ```

    The port will typically be in the range 30000–32767.


6. Run the script to generate connection requests at regular intervals.

    ```bash
    sh script.sh
    ``` 

5. In the Web UI, you can view the name of the pod of TCP server application.


