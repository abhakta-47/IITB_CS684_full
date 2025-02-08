FROM ubuntu:22.04

RUN apt update -y
RUN apt install -y wget
RUN apt install -y m4 make wget patch unzip bubblewrap git g++
RUN apt install -y libgtk2.0-dev
RUN apt install -y curl
RUN apt install -y zsh sudo ocaml

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
RUN . $LUSTRE_INSTALL/setenv.sh

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
RUN sudo apt install -y camlp4-extra
RUN eval $(opam env --switch=4.05.0) && opam install -y menhir lablgtk ocamlgraph ocamlbuild camlp4 
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

RUN sudo apt install -y libxt6 libxt-dev
RUN sudo mkdir -p /volume1/ && sudo chown -R user:usergroup /volume1/

COPY home/.zshrc /home/user
COPY build_run /usr/local/bin
RUN sudo chmod +x /usr/local/bin/build_run

CMD ["tail", "-f", "/dev/null"]
