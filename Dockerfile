FROM ubuntu:latest


WORKDIR /app
COPY web /app/web
COPY requirements.txt CMakeLists.txt init_container.sh /app/
COPY src /app/src
COPY include /app/include

RUN apt-get update && apt-get install -y \
    build-essential \
    libzstd-dev \
    llvm \
    cmake \
    python3 \
    python3-pip \
    python3-venv \
    clang

COPY sshd_config /etc/ssh/
RUN apt-get update \
    && apt-get install -y --no-install-recommends dialog \
    && apt-get install -y --no-install-recommends openssh-server \
    && echo "root:Docker!" | chpasswd \
    && chmod u+x /app/init_container.sh

ENV PYTHONUNBUFFERED=1 \
    VIRTUAL_ENV=/opt/venv

RUN python3 -m venv $VIRTUAL_ENV && \
    $VIRTUAL_ENV/bin/pip install --upgrade pip
    
ENV PATH="$VIRTUAL_ENV/bin:$PATH"
RUN pip install --no-cache-dir -r requirements.txt

EXPOSE 8000 2222

RUN mkdir -p build
RUN cd build && cmake ..
RUN cd build && cmake --build .

ENTRYPOINT [ "/app/init_container.sh" ] 
CMD ["python", "web/app.py"]
