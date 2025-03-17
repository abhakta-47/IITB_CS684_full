FROM ubuntu:22.04

RUN apt update -y
RUN apt install -y \
    wget \
    m4 \
    make \
    wget \
    patch \
    unzip \
    bubblewrap \
    git \
    g++ \
    libgtk2.0-dev \
    curl \
    zsh \
    sudo \
    ocaml \
    locales \
    python3-venv \
    camlp4-extra \
    libxt6 \
    libxt-dev
RUN locale-gen en_US.UTF-8
RUN update-locale LANG=en_US.UTF-8

# Create a new user with UID 1001 and add to sudo group
RUN groupadd -g 1001 usergroup
RUN useradd -m -u 1000 -g 1001 -d /home/user -s /bin/zsh user && \
    echo 'user ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
# change user
USER user

WORKDIR /build

# install lustre
ARG LUSTRE_VER="lustre-v4-IV.2024.144-linux64"
RUN wget "https://www-verimag.imag.fr/DIST-TOOLS/SYNCHRONE/lustre-v4/distrib/linux64/$LUSTRE_VER.tgz" -O lustre.tgz
RUN tar xvf lustre.tgz
RUN sudo cp -R $LUSTRE_VER /usr/local/
ENV LUSTRE_INSTALL=/usr/local/$LUSTRE_VER
RUN . $LUSTRE_INSTALL/setenv.sh && unset LUSTRE_INSTALL

# Install opam
RUN wget https://raw.githubusercontent.com/ocaml/opam/master/shell/install.sh -O opam-install.sh
RUN chmod +x opam-install.sh
RUN printf "\n" | ./opam-install.sh
RUN opam init --disable-sandboxing -a
RUN opam switch create 4.05.0

# Install heptagon
RUN wget --no-check-certificate 'https://docs.google.com/uc?export=download&id=1Ru1kdpqEAKQ-BJX_QyQpbGsxy7fqpWaI' -O hepta.zip
RUN unzip hepta.zip
WORKDIR /build/heptagon-1.05.00
COPY reset.ml compiler/heptagon/trasnformation/reset.ml
RUN eval $(opam env --switch=4.05.0) && opam install -y menhir lablgtk ocamlgraph ocamlbuild camlp4 
COPY hepts.ml compiler/main/hepts.ml
RUN eval $(opam env --switch=4.05.0) && ./configure && make
RUN sudo make install

# configure zsh etc
# Change default shell to zsh
RUN sudo chsh -s $(which zsh)
# Enable zsh history
# Install zsh plugins
RUN git clone --depth=1 https://github.com/zsh-users/zsh-autosuggestions ~/.zsh/zsh-autosuggestions
RUN git clone --depth=1 https://github.com/zsh-users/zsh-syntax-highlighting.git ~/.zsh/zsh-syntax-highlighting

# RUN git clone --depth=1 https://github.com/romkatv/powerlevel10k.git ~/.zsh/powerlevel10k
# RUN echo "source ~/.zsh/powerlevel10k/powerlevel10k.zsh-theme" >>~/.zshrc

RUN sudo mkdir -p /volume1/ && sudo chown -R user:usergroup /volume1/
RUN cd /usr/local && curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sudo sh
RUN arduino-cli core install arduino:avr

COPY home/.zshrc /home/user
COPY build_run /usr/local/bin
RUN sudo chmod +x /usr/local/bin/build_run
RUN mkdir -p /home/user/.local/share/

CMD ["tail", "-f", "/dev/null"]
