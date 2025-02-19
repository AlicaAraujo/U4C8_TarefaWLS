#include "ssd1306.h"

// Funções

/*
* Função para inicializar o display SSD1306.
*
* Configura as dimensões do display, o endereço I2C e cria os buffers necessários para 
* o envio de dados ao display.
*
* Parâmetros: 
* - ssd: Ponteiro para a estrutura do display.
* - width: Largura do display em pixels.
* - height: Altura do display em pixels.
* - external_vcc: Indica se o display utiliza alimentação externa (true) ou interna (false).
* - address: Endereço I2C do display.
* - i2c: Ponteiro para a instância I2C utilizada.
*/
void ssd1306_init(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c) {
    ssd->width = width;
    ssd->height = height;
    ssd->pages = height / 8U;
    ssd->address = address;
    ssd->i2c_port = i2c;
    ssd->bufsize = ssd->pages * ssd->width + 1;
    ssd->ram_buffer = calloc(ssd->bufsize, sizeof(uint8_t));
    ssd->ram_buffer[0] = 0x40;
    ssd->port_buffer[0] = 0x80;
}

/*
* Função para configurar o display SSD1306 com os parâmetros necessários.
*
* Envia uma sequência de comandos ao display para configurar a memória de exibição, 
* contraste, orientação e outras configurações essenciais para o funcionamento adequado.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
*/
void ssd1306_config(ssd1306_t *ssd) {
    ssd1306_command(ssd, SET_DISP | 0x00);
    ssd1306_command(ssd, SET_MEM_ADDR);
    ssd1306_command(ssd, 0x01);
    ssd1306_command(ssd, SET_DISP_START_LINE | 0x00);
    ssd1306_command(ssd, SET_SEG_REMAP | 0x01);
    ssd1306_command(ssd, SET_MUX_RATIO);
    ssd1306_command(ssd, HEIGHT - 1);
    ssd1306_command(ssd, SET_COM_OUT_DIR | 0x08);
    ssd1306_command(ssd, SET_DISP_OFFSET);
    ssd1306_command(ssd, 0x00);
    ssd1306_command(ssd, SET_COM_PIN_CFG);
    ssd1306_command(ssd, 0x12);
    ssd1306_command(ssd, SET_DISP_CLK_DIV);
    ssd1306_command(ssd, 0x80);
    ssd1306_command(ssd, SET_PRECHARGE);
    ssd1306_command(ssd, 0xF1);
    ssd1306_command(ssd, SET_VCOM_DESEL);
    ssd1306_command(ssd, 0x30);
    ssd1306_command(ssd, SET_CONTRAST);
    ssd1306_command(ssd, 0xFF);
    ssd1306_command(ssd, SET_ENTIRE_ON);
    ssd1306_command(ssd, SET_NORM_INV);
    ssd1306_command(ssd, SET_CHARGE_PUMP);
    ssd1306_command(ssd, 0x14);
    ssd1306_command(ssd, SET_DISP | 0x01);
}

/*
* Função para enviar um comando ao display SSD1306 via I2C.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
* - command: Comando a ser enviado ao display.
*/
void ssd1306_command(ssd1306_t *ssd, uint8_t command) {
    ssd->port_buffer[1] = command;
    i2c_write_blocking(
        ssd->i2c_port,
        ssd->address,
        ssd->port_buffer,
        2,
        false);
}

/*
* Função para enviar os dados do buffer RAM para o display SSD1306.
*
* Atualiza o display enviando os dados armazenados no buffer interno via I2C.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
*/
void ssd1306_send_data(ssd1306_t *ssd) {
    ssd1306_command(ssd, SET_COL_ADDR);
    ssd1306_command(ssd, 0);
    ssd1306_command(ssd, ssd->width - 1);
    ssd1306_command(ssd, SET_PAGE_ADDR);
    ssd1306_command(ssd, 0);
    ssd1306_command(ssd, ssd->pages - 1);
    i2c_write_blocking(
        ssd->i2c_port,
        ssd->address,
        ssd->ram_buffer,
        ssd->bufsize,
        false);
}

/*
* Função para definir o estado de um pixel no buffer do display.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
* - x: Coordenada X do pixel.
* - y: Coordenada Y do pixel.
* - value: Estado do pixel (true para ligado, false para desligado).
*/
void ssd1306_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool value) {
    uint16_t index = (y >> 3) + (x << 3) + 1;
    uint8_t pixel = (y & 0b111);
    if (value)
        ssd->ram_buffer[index] |= (1 << pixel);
    else
        ssd->ram_buffer[index] &= ~(1 << pixel);
}

/*
* Função para preencher todo o display com um único valor (ligado ou desligado).
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
* - value: Estado para preencher o display (true para ligado, false para desligado).
*/
void ssd1306_fill(ssd1306_t *ssd, bool value) {
    for (uint8_t y = 0; y < ssd->height; ++y) {
        for (uint8_t x = 0; x < ssd->width; ++x) {
            ssd1306_pixel(ssd, x, y, value);
        }
    }
}

/*
* Função para desenhar um retângulo no display.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
* - top: Coordenada Y do canto superior do retângulo.
* - left: Coordenada X do canto esquerdo do retângulo.
* - width: Largura do retângulo.
* - height: Altura do retângulo.
* - value: Estado do pixel (true para ligado, false para desligado).
* - fill: Se true, preenche o retângulo; se false, desenha apenas as bordas.
*/
void ssd1306_rect(ssd1306_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value, bool fill) {
    for (uint8_t x = left; x < left + width; ++x) {
        ssd1306_pixel(ssd, x, top, value);                    // Linha superior
        ssd1306_pixel(ssd, x, top + height - 1, value);       // Linha inferior
    }

    for (uint8_t y = top; y < top + height; ++y) {
        ssd1306_pixel(ssd, left, y, value);                   // Linha esquerda
        ssd1306_pixel(ssd, left + width - 1, y, value);       // Linha direita
    }
}

/*
* Função para desenhar uma borda de estrelas no display.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
* - top: Coordenada Y do canto superior da borda.
* - left: Coordenada X do canto esquerdo da borda.
* - width: Largura da borda.
* - height: Altura da borda.
* - value: Estado do pixel (true para ligado, false para desligado).
*/
void ssd1306_rect_hearts(ssd1306_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value) {
    const uint8_t spacing = 5; // Espaçamento entre as estrelas

    for (uint8_t x = left; x < left + width; x += spacing) {
        ssd1306_draw_heart(ssd, x, top, value);                    // Linha superior
        ssd1306_draw_heart(ssd, x, top + height - 1, value);       // Linha inferior
    }

    for (uint8_t y = top; y < top + height; y += spacing) {
        ssd1306_draw_heart(ssd, left, y, value);                   // Linha esquerda
        ssd1306_draw_heart(ssd, left + width - 1, y, value);       // Linha direita
    }
}

/*
* Função para desenhar uma estrela no display.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
* - x: Coordenada X do centro da estrela.
* - y: Coordenada Y do centro da estrela.
* - value: Estado do pixel (true para ligado, false para desligado).
*/
void ssd1306_draw_heart(ssd1306_t *ssd, uint8_t x, uint8_t y, bool value) {
    ssd1306_pixel(ssd, x - 1, y, value);
    ssd1306_pixel(ssd, x + 1, y, value);
    
    ssd1306_pixel(ssd, x - 2, y + 1, value);
    ssd1306_pixel(ssd, x - 1, y + 1, value);
    ssd1306_pixel(ssd, x, y + 1, value);
    ssd1306_pixel(ssd, x + 1, y + 1, value);
    ssd1306_pixel(ssd, x + 2, y + 1, value);

    ssd1306_pixel(ssd, x - 2, y + 2, value);
    ssd1306_pixel(ssd, x - 1, y + 2, value);
    ssd1306_pixel(ssd, x, y + 2, value);
    ssd1306_pixel(ssd, x + 1, y + 2, value);
    ssd1306_pixel(ssd, x + 2, y + 2, value);

    ssd1306_pixel(ssd, x - 1, y + 3, value);
    ssd1306_pixel(ssd, x, y + 3, value);
    ssd1306_pixel(ssd, x + 1, y + 3, value);

    ssd1306_pixel(ssd, x, y + 4, value);
}

/*
* Função para desenhar uma linha entre dois pontos no display.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
* - x0: Coordenada X do ponto inicial.
* - y0: Coordenada Y do ponto inicial.
* - x1: Coordenada X do ponto final.
* - y1: Coordenada Y do ponto final.
* - value: Estado do pixel (true para ligado, false para desligado).
*/
void ssd1306_line(ssd1306_t *ssd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        ssd1306_pixel(ssd, x0, y0, value);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = err * 2;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

/*
* Função para desenhar uma linha horizontal no display.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
* - x0: Coordenada X inicial.
* - x1: Coordenada X final.
* - y: Coordenada Y da linha.
* - value: Estado do pixel (true para ligado, false para desligado).
*/
void ssd1306_hline(ssd1306_t *ssd, uint8_t x0, uint8_t x1, uint8_t y, bool value) {
    for (uint8_t x = x0; x <= x1; ++x)
        ssd1306_pixel(ssd, x, y, value);
}

/*
* Função para desenhar uma linha vertical no display.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
* - x: Coordenada X da linha.
* - y0: Coordenada Y inicial.
* - y1: Coordenada Y final.
* - value: Estado do pixel (true para ligado, false para desligado).
*/
void ssd1306_vline(ssd1306_t *ssd, uint8_t x, uint8_t y0, uint8_t y1, bool value) {
    for (uint8_t y = y0; y <= y1; ++y)
        ssd1306_pixel(ssd, x, y, value);
}
