# Client_server_graph


# Как развернуть сервис на площадке play with docker:
1. Регистрация:
    <li>
    * зарегистрируйтесь на сервисе Docker Hub: [DockerHub](https://hub.docker.com), вы создадите новый Docker ID
    * Перейдите по ссылке: https://labs.play-with-docker.com
    * Авторизируйтесь с помощью только что полученного Docker ID
    * Вы создадите сессию, длинной в 4 часа

2. Добавление хостов:
    Вы можете создавать хосты с помощью кнопки "+ ADD NEW INSTANCE".
    Слева отражен список всех хостов и их адреса, вы можете переключаться между хостами.
    Вам потребуется два хоста: клиент и сервер

3. Развертывание сервера:  
    Добавьте новый хост и введите в терминал следующую команду:
    docker run -p 49150:49150 p0l0satik/server_app 
    эта команда скачает из репозитория образ сервера и запустит его в контейнере docker
    -p связывает порты контейнера и хоста, в данном случае это порт 49150, важно чтобы сервер находился именно на этом порте

4. Развертывание клиента:
    Добавьте новый хост и введите в терминал следующую команду:
    docker run -it -p 8080:8080 p0l0satik/client_app 
    эта команда скачает из репозитория образ клиента и запустит его в контейнере docker
    -it позволит вам взаимодействовать с клиентом
    порт клиента нам не важен, но необходимо опубликовать хотя бы один порт клиентского контейнера

    когда клиент запустится, он попросит вас ввести IP адресс сервера, введите адрес того хоста на котором запущен 
    контейнер сервера

# Как запустить многопользовательское приложение с использованием docker swarm
1. Сессия
    Создание сессию на площадке: https://labs.play-with-docker.com
2. Инициализация swarm
    Создайте хост и запустите swarm командой:
    docker swarm init --advertise-addr %ip_of_your_host%
    вместо %ip_of_your_host% вам нужно подставить IP вашего хоста
3. Добавление работников
    есть два типа узлов - менеджер и работник. Предыдущей командой вы создали менеджера который будет управлять swarm. 
    менеджер выдал вам строку вида:
    docker swarm join --token SWMTKN-1-0j7pjebjsrhj9fbndqp2c78ql1klstpp3hfpmua02egzcn3w9i-796r0dh0gn0llnhfeux94etdr   192.168.0.17:2377
    это команда добавления узла-работника к swarm
    создайте хост и скопируйте ее в терминал этого хоста
4. Запуск сервиса
    командой docker service create --name=server --publish published=49150,target=49150 --replicas=2 p0l0satik/server_app
    вы создадите и запустите две копии контейнера p0l0satik/server_app
    swarm автоматически старается распределить контейнеры так чтобы на каждом хосте была только одна копия
    как менеджеры так и рабочие могут запускать контейнеры, в данном случае один контейнер будет запущен на рабочем, второй на менеджере
5. Несколько клиентов
    создайте еще два хоста и в каждом запустите клиентский контейнер командой:
    docker run -it -p 8080:8080 p0l0satik/client_app 
    в качестве IP адреса обоим подайте адресс менеджера(самый первый хост)
    попробуйте вбить какие-нибудь графы и проверить работает ли приложение
6. Один хост - один клиент
    Из-за статической привязки портов для каждого клиента требуется один хост(в данном примере, если попробовать подключить третьего клиента его работа будет некорректной!)
    командой docker service scale имя_сервиса=количество_копий
    однако если на одном хосте будет два или больше серверных приложений работать они будут как один из-за статической проброске портов
    
 
