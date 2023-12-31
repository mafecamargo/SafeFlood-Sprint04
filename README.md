# SafeFlood-Sprint04

Objetivo do Safe Flood:
<br>
Resolvemos desenvolver um aplicativo móvel que alerte e notifique os moradores de casas e estabelecimentos sobre a possibilidade de enchentes em suas áreas é uma iniciativa crucial para promover a segurança das comunidades. Esse aplicativo fornecerá informações em tempo real, baseadas em dados meteorológicos e de monitoramento hidrológico, permitindo que as pessoas se preparem de forma adequada, como mover seus pertences para locais elevados, desligar eletrônicos vulneráveis à água e até mesmo evacuar, se necessário. 
<br>
Além disso, ao aumentar a conscientização sobre os riscos de enchentes e promover ações preventivas, como a criação de planos de emergência familiares, o aplicativo contribui para a redução de danos materiais, riscos à vida e prejuízos econômicos, promovendo a resiliência das comunidades diante de eventos climáticos extremos. 
<br> <br>
# Link para visualizar os slides: <br>
https://fiapcom-my.sharepoint.com/:p:/g/personal/rm97956_fiap_com_br/Ea7g0RSlwclGsL-ZmFB2AnIBn_UooVSiVNONsGb-1Op5yQ?e=KwmTfG
# Link para visualizar o circuito com o ESP-32: <br>
https://wokwi.com/projects/380675588769496065

# Elementos do circuito: <br>
- 1 ESP-32 <br>
- 1 Sensor Ultrassônico (echo_pin 21; trig_pin 19) <br>
- 1 DHT (pin_dht 12) <br>
- 1 LED Vermelho (red_led 4) <br>
- 1 LED Amarelo (yellow_led 2) <br>
- 1 LED Verde (green_led 15) <br>
- 3 Resistores (220 Ω)

# Circuito e MQTT:<br>
Durante o código do circuito no Wokwi, declaramos um Broker MQTT (broker.hivemq.com) e um tópico por onde as informações serão publicadas (FIAP/SPRINT/TempHumi e FIAP/SPRINT/Distance).
 <br>
Após esse processo, ligamos o broker ao HIVE MQ, em que as informações do circuito serão ṕublicadas a partir dos tópicos.
 <br>
Abrimos o node-red, para passar as informações que o Hive está recebendo para o Tago.io
<br>
Criamos um device no tago.io, e após ajeitarmos as informações que o nó de mqtt-out do node-red pede, entramos em Live Inspector - onde as informações do circuito são mostradas.

# Video - Arduino e Python: <br>
Utilizamos uma versão anterior do nosso circuito - sem o sensor DHT11 - para demonstrar como seria o circuito na sua versão física e ligamos o Arduino ao código de Python.
<br>
Dessa forma, podemos demonstrar como o nosso aplicativo mostraria as informações dos sensores - nesse caso o sensor Ultrassônico - com a mudança da distância e a preocupação que os usuários devem ter conforme as luzes acendem.
<br>
- Se a luz verde estiver acesa, signifca que está seguro; <br>
- Se a luz amarela estiver acesa, significa que o risco é moderado; <br>
- Se a luz vermelha estiver acesa, significa que o risco é alto.
