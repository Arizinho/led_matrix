# Animação em Matriz de LEDs da BitDogLab

Este projeto utiliza um teclado matricial para controlar a matriz de LEDs da **BitDogLab**. As funções disponíveis incluem a exibição de uma animação, o acionamento da matriz em diferentes cores e intensidades, além de comandos para controle do modo de armazenamento da Raspberry Pi Pico W.

---

## **Configuração dos pinos GPIO** 
Os pinos GPIO da Raspberry Pi Pico W estão configurados conforme a tabela abaixo: 

| GPIO | Componente | Função |  
|------------|----------------|---------------------------------------------------------|  
| **7** | Matriz de LEDs | Controla o padrão de cores exibido na matriz de LEDs (saída) |  
| **2, 3, 4, 5** | Linhas do teclado matricial | Envia sinais para as linhas do teclado matricial (saída) |  
| **16, 17, 18, 19** | Colunas do teclado matricial | Recebe sinais das colunas do teclado matricial (entrada com pull-down) |  

---

## **Funcionamento do Teclado Matricial**  

| Tecla | Função |  
|-------|------------------------------------------------------|  
| `1` | Exibe uma animação curta de um gato na matriz de LEDs |  
| `0 e 2-9` | Exibe **"Animação indisponível"** via comunicação serial |  
| `A` | Desliga todos os LEDs |  
| `B` | Acende todos os LEDs em **azul** com **100%** de intensidade |  
| `C` | Acende todos os LEDs em **vermelho** com **80%** de intensidade |  
| `D` | Acende todos os LEDs em **verde** com **50%** de intensidade |  
| `#` | Acende todos os LEDs em **branco** com **20%** de intensidade |  
| `*` | Coloca a Raspberry Pi Pico W no modo de armazenamento |  

---

## **Link do Vídeo**  

[https://drive.google.com/file/d/1VXEfgnm6a1NYm3uRt1uxuv77K7U9XTPa/view?usp=sharing]
