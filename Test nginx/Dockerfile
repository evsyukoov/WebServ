FROM nginx:alpine

RUN mkdir /usr/share/nginx/html/YoupiBanane
RUN touch /usr/share/nginx/html/YoupiBanane/youpi.bad_extension
RUN touch /usr/share/nginx/html/YoupiBanane/youpi.bla
RUN mkdir /usr/share/nginx/html/YoupiBanane/nop
RUN touch /usr/share/nginx/html/YoupiBanane/nop/youpi.bad_extension
RUN touch /usr/share/nginx/html/YoupiBanane/nop/other.pouic
RUN mkdir /usr/share/nginx/html/YoupiBanane/Yeah
RUN touch /usr/share/nginx/html/YoupiBanane/Yeah/not_happy.bad_extension

EXPOSE 80
COPY start.sh /start.sh
COPY index.html /usr/share/nginx/html/index.html
COPY conf.d/default.conf /etc/nginx/default.conf 

CMD sh /start.sh
