# Projeto ESP32 com LED RGB, Sensor LDR e Sensor DHT 11
Este projeto utiliza uma placa ESP32, um LED RGB de 4 pinos, um sensor LDR de 5mm e outros componentes para criar um sistema que exibe a temperatura, umidade e luminosidade via uma interface web. O sistema também indica o status da conexão Wi-Fi através de cores no LED RGB. Além disso, o projeto gera um JSON com os dados, que é atualizado a cada requisição GET, permitindo integração com o Zabbix.

## Funcionalidades
Medição de Temperatura e Umidade: Utiliza um sensor DHT11 para capturar e exibir a temperatura e umidade em tempo real.
Medição de Luminosidade: Utiliza um LDR de 5mm para medir a luminosidade ambiente.
Interface Web: Exibe os dados capturados pelo DHT11 e LDR através de uma interface web acessível via navegador.
Geração de JSON: Gera um JSON com os dados capturados para integração com o Zabbix.
Indicação de Status do Wi-Fi: O LED RGB muda de cor para indicar o status da conexão Wi-Fi:
Verde: Conectado ao Wi-Fi.
Vermelho piscando: Falha ao conectar ao Wi-Fi.
Transição de Cores: O LED passa por todas as cores disponíveis se não estiver conectado ao Wi-Fi.

### Web
![WEB](/imagens/web.png)

### JSON
![JSON](/imagens/webjson.png)

### Protótipo
![Final](/imagens/espfinal.jpeg)

## Materiais Utilizados
### ESP32-WROOM-32U com Antena Externa
![Esp32](/imagens/esp32.jpeg)

### Cabo Micro USB e Plug 5V para Alimentação e Programação
![Micro USB](/imagens/microusb.jpeg)

### Jumpers Fêmea-Fêmea
![Jumper](/imagens/jumperfemea.jpeg)

### Caixa de Proteção Intelbras para CFTV
![Caixa Proteção](/imagens/caixapassagem.jpeg)

### LDR (Resistor Dependente de Luz) 5mm
![LDR](/imagens/ldr.jpeg)

### Resistor de 10Ω
![Resistor](/imagens/resistor.jpeg)

### LED RGB de 4 Pinos
![LED](/imagens/ledrgb.jpeg)

Montagem Final: Todos os componentes foram estruturados dentro de uma caixa de passagem para CFTV da Intelbras, garantindo proteção e organização.

## Conexões

### LED RGB:
1. Pino Vermelho (R): GPIO 16
2. Pino Verde (G): GPIO 17
3. Pino Azul (B): GPIO 5
4. Pino Comum (Catodo/Anodo): GND ou VCC dependendo do tipo do LED (catodo comum ou anodo comum).

### LDR:
1. Um pino do LDR: GPIO 34 (leitura analógica).
2. Outro pino do LDR: 3.3V.
3. Resistor de Pull-down: 10Ω entre GPIO 34 e GND.

### DHT11:
1. Pino de Dados: GPIO 4.
2. VCC: 3.3V.
3. GND: GND.

## Instalação e Configuração
Clone o repositório:

## Copiar código
```
git clone https://github.com/Kh4os-afk/DHT-ESP32
```
### Abra o projeto no Arduino IDE ou PlatformIO.
1. Substitua as credenciais de Wi-Fi no código (ssid e password).
2. Conecte a ESP32 ao seu computador via cabo USB.
3. Importe todas as bibliotecas necessarias:
   - [Adafruit Sensor](/libraries/Adafruit_sensor)
   - [Async TCP](/libraries/AsyncTCP)
   - [DHT Sensor](/libraries/DHT_sensor)
   - [ESP Async Web Server](/libraries/ESPAsyncWebServer) 
5. Compilar o código para a ESP32.
6. Acesse a interface web via o IP configurado (ex: http://127.0.0.1).
5.Acesse a interface web JSON via o IP configurado (ex: http://127.0.0.1/dht).

## Integração com Zabbix
O projeto gera um JSON dos dados de temperatura, umidade e luminosidade. Esse JSON é atualizado em cada requisição e pode ser integrado com o Zabbix para monitoramento contínuo.

Para facilitar essa integração, um template Zabbix foi incluído no repositório:

Arquivo de Template: [sensor-dht-ldr-esp-32.yaml](template%20zabbix/sensor_dht_ldr_esp_32.yaml)

### Como usar:
1. Importe o template no Zabbix.
2. Configure um host com o ip do ESP
3. Aplique o template no host

![Zabbix](/imagens/zabbix.png)


## Funcionamento
Ao ligar o dispositivo: Ele tentará se conectar ao Wi-Fi. Se a conexão for bem-sucedida, o LED ficará verde. Se falhar, o LED piscará em vermelho.
Acesso à interface web: Use um navegador para acessar o IP da ESP32 e visualizar as leituras de temperatura, umidade e luminosidade em tempo real.
Transição de Cores: Se o dispositivo não estiver conectado ao Wi-Fi, o LED RGB passará por uma transição suave entre várias cores.

## Equipe
:mage: [Lucas Tabosa](https://www.linkedin.com/in/lucastabosa/)
:man_office_worker: [Joelson Martins](https://www.linkedin.com/in/joelsonmartins/)
:sunglasses: [Bruno Silva](https://www.linkedin.com/in/brunosilvait/)
:robot: [Augusto Oliveira](https://www.linkedin.com/in/augusto-oliveira-a352a31a4/)
:cowboy_hat_face: [Daniel Barbosa](https://www.linkedin.com/in/daniel-barbosa-aba455113/)
:hurtrealbad: [João Pedro](https://www.linkedin.com/in/joão-pedro-aa62bb21b/)
