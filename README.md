# Arduino SNMP (Simple Network Management Protocol)


Este projeto implementa o protocolo de rede SNMP em uma placa Arduino, utilizando a biblioteca *agentuino.* 

A implementação do SNMP no arduino, abre possibilidades de monitoramento com variados sensores(umidade, temperatura, fumaça, luminosidade, presença etc..), podendo ser integrado com qualquer ferramenta que leia mibs snmp, por exemplo o Zabbix, nagios, prometheus e etc.   

Neste exemplo, implementaremos uma mib snmp em uma placa arduino Uno para monitoramento de temperatura e umidade com o sensor DHT11.


## Componentes


* Arduino Uno
* Ethernet Shield W5100
* Sensor de temperatura e umidade DHT11.


## Montagem do Circuito.


Podemos usar um protoboard para experimento ou diretatemente no sensor(DHT11).

-- Circuito com ProtoBoard.

![alt text](https://github.com/lailtonmontenegro/arduino-snmp/blob/main/Circuito-protoboad.jpeg)

-- Circuito sem o ProtoBoard.

![alt text](https://github.com/lailtonmontenegro/arduino-snmp/blob/main/Circuito-SemProtoboard.jpeg)


## Importando a biblioteca Agentuino.


Precisamos importar o arquivo *Agentuino.zip* deste projeto na IDE arduino. 

Segue o caminho. 

Sketch>Include Library>ADD .ZIP Library.


## Alguns detalhes.


* [ Alterando IP e Faixa de rede] Antes de enviar o código para placa, é preciso editar o ip e máscara para faixa de rede local. (Linha 15)
* [ Editando a Comunidade ] A comunidade esta setada como *public*, mas podemos edita-lo no *Agentuino.cpp* biblioteca que esta no pacote .zip deste projeto.( Linha 32)
* [ IDs da MIB ] Na MIB o ID para temperatura é 1.3.6.1.4.1.36582.3.1.0, para umidade é 1.3.6.1.4.1.36582.3.2.0 ambas Read-only.
* [ Snmp version] A versão setada como default é a snmpv1. Conseguimos testar a v2, mas ainda não evoluimos na implementação para o snmpv3.


## Validando


Após envio do código para placa, conseguimos consultar as informações do sensor utilizando o comando snmpget. 


* Temperatura

```shell
snmpget -v 1 -r 1 -c public 192.168.0.168 1.3.6.1.4.1.36582.3.1.0
```


* Umidade

```shell
snmpget -v 1 -r 1 -c public 192.168.0.168 1.3.6.1.4.1.36582.3.2.0
```

Exemplo de resultado do snmpget para os ids de temperatura e umidade respectivamente.


![alt text](https://github.com/lailtonmontenegro/arduino-snmp/blob/main/Exemploresultado.jpeg)


## Algumas referências. 


https://www.slideshare.net/lailtonmontenegro/monitoramento-de-temperatura-e-umidade-de-data-center-utilizando-o-arduino-e-o-sistema-zabbix

https://code.google.com/archive/p/agentuino/

https://en.wikipedia.org/wiki/Simple_Network_Management_Protocol

https://www.arduino.cc/en/software



### Se tiver alguma nova ideia, sugestões de correção ou melhoria, fique a vontade para contribuir. :) 
