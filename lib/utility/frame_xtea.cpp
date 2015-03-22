#include "frame_xtea.h"

void frame_xtea(int32_t* v, int32_t* o, int32_t* k, int32_t n)
{
    uint32_t y = v[0];
    uint32_t z = v[1];
    uint32_t delta = 0x9e3779b9; // 0x00000000 - 0x61C88647 == 0x9e3779b9

    if (n > 0)
    {
        // Encoding
        uint32_t limit = delta * n;
        uint32_t sum = 0;
        while (sum != limit)
        {
            y += (((z << 4) ^ (z >> 5)) + z) ^ (sum + k[sum & 3]);
            sum += delta;
            z += (((y << 4) ^ (y >> 5)) + y) ^ (sum + k[(sum>>11) & 3]);
        }
    }
    else
    {
        // Decoding
        uint32_t sum = delta * (-n);
        while (sum)
        {
            z -= (((y << 4) ^ (y >> 5)) + y) ^ (sum + k[(sum>>11) & 3]);
            sum -= delta;
            y -= (((z << 4) ^ (z >> 5)) + z) ^ (sum + k[sum & 3]);
        }
    }

    o[0] = y;
    o[1] = z;
}

/*****************************************************************************
//	���ܺ��Buffer�ṹ
//  ������-----���������Щ�������������������--------����---���������Щ�---��������
//  �� PadLength   ��  Padded Random BYTEs ��  Data  �� Zero s ��
//  ��������������-----�੤������������--------���������੤---�������੤����---����
//  ��    1Byte    ��    PadLength Bytes   �� N Bytes�� 7Bytes ��
//  ������������-----���ة�������������--------���������ة�����---���ة�������---��
// Pad�������ڽ�����Buffer���뵽8�ֽڶ���
******************************************************************************/
size_t frame_xtea_encrypt(char* buf_in, size_t buf_in_len, char* buf_out, size_t buf_out_len, const char key[16])
{
    if (buf_in == NULL || buf_in_len <= 0)
    {
        return 0;
    }
    //������Ҫ�����buffer��С
    size_t zero = 1 + buf_in_len + ZERO_LENGTH;
    size_t pad_len = zero % ENCRYPT_BLOCK_LENGTH_IN_BYTE;
    size_t i;

    if (pad_len != 0)
    {
        pad_len = ENCRYPT_BLOCK_LENGTH_IN_BYTE - pad_len;
    }
    size_t total_len = zero + pad_len;

    if (total_len > buf_out_len || buf_out == NULL)
    {
        return -1;
    }

    char* ptmp_in = buf_in;
    char* ptmp_out = buf_out;
    memset(buf_out, 0, buf_out_len);

    char first[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
    //��һ��Ԫ�أ�ֻʹ�����3λ����������������
    first[0] = (((char)rand()) & 0xf8) | (char)pad_len;
    //�������������µ�pad����
    for (i=1; i <= pad_len; ++i)
    {
        first[i] = (char)rand();
    }
    //�ô��������ݲ����һ������
    for (i = 1+pad_len; i < ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
    {
        first[i] = *ptmp_in++;
    }
    // ��һ�����ܿ�����������ģ����ں����������
    char* plast_encrypt = buf_out;
    char* plast_plain = first;

    // ��һ��Buffer������Ҫ������
    frame_xtea((int32_t*) first, (int32_t*)ptmp_out, (int32_t*)key, ENCRYPT_ROUNDS);
    ptmp_out += ENCRYPT_BLOCK_LENGTH_IN_BYTE;

    // ��������������ڲ�����InBuffer�ļ��ܹ���
    char src_buf[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
    while ((ptmp_in - buf_in) < (buf_in_len - 1))
    {
        memcpy(src_buf, ptmp_in, ENCRYPT_BLOCK_LENGTH_IN_BYTE);
        //����һ���������
        int32_t n ;
        for (n = 0; n < ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++n)
        {
            src_buf[n] ^= plast_encrypt[n];
        }
        frame_xtea((int32_t*)src_buf, (int32_t*)ptmp_out, (int32_t*)key, ENCRYPT_ROUNDS);
        //����һ���������
        for (n = 0; n < ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++n)
        {
            ptmp_out[n] ^= plast_plain[n];
        }
        plast_encrypt = ptmp_out;
        plast_plain = ptmp_in;

        ptmp_in += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
        ptmp_out += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
    }

    // ��β�� 1Byte���� + 7Byte У��
    char last[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
    memset(&last[0], 0, ENCRYPT_BLOCK_LENGTH_IN_BYTE);
    last[0] = *ptmp_in;

    //����һ���������
    for (i=0; i < ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
    {
        last[i] ^= plast_encrypt[i];
    }
    frame_xtea((int32_t*)last, (int32_t*)ptmp_out, (int32_t*)key, ENCRYPT_ROUNDS);
    //����һ���������
    for (i=0; i < ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
    {
        ptmp_out[i] ^= plast_plain[i];
    }

    return total_len;
}

size_t frame_xtea_decrypt(char* buf_in, size_t buf_in_len, char* buf_out, size_t buf_out_len, const char key[16])
{
    if (buf_in == NULL || buf_in_len <= 0)
    {
        return 0;
    }
    // Buffer����Ӧ�����ܱ� ENCRYPT_BLOCK_LENGTH_IN_BYTE ������
    if (buf_in_len % ENCRYPT_BLOCK_LENGTH_IN_BYTE || buf_in_len <= 8)
    {
        return -1;
    }

    char* ptmp_in = buf_in;
    char* ptmp_out = buf_out;
    size_t i;

    // �Ƚ����ǰ�����Pad��ENCRYPT_BLOCK_LENGTH_IN_BYTE��Byte
    char first[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
    frame_xtea((int32_t*)ptmp_in, (int32_t*)first, (int32_t*)key, DECRYPT_ROUNDS);
    ptmp_in += ENCRYPT_BLOCK_LENGTH_IN_BYTE;

    // Pad����ֻ�����˵�һ���ֽڵ����3bit����5bit�������
    int32_t pad_len = first[0] & 0x07;
    // ����ԭʼ���ݵĳ���
    size_t plain_len = buf_in_len - 1/*PadLength Length*/ - pad_len - ZERO_LENGTH;
    if (plain_len <= 0 || buf_out == NULL)
    {
        return -2;
    }
    //out buffer ����
    if (plain_len > buf_out_len)
    {
        return -3;
    }
    //ǰһ������ĺ����ģ����ں����������
    char* plast_encrypt = buf_in;
    char* plast_plain = first;

    //����һ����Pad��Ϣ֮��������Ƶ����Buffer
    for ( i=0; i < 7/*ENCRYPT_BLOCK_LENGTH_IN_BYTE - 1*/ - pad_len; ++i)
    {
        *ptmp_out++ = first[1 + pad_len + i];
    }
    //���ܳ������һ����������п�
    char src_buf[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};

    while ((ptmp_in - buf_in) < (buf_in_len - ENCRYPT_BLOCK_LENGTH_IN_BYTE))
    {
        int32_t n;
        memcpy(src_buf, ptmp_in, ENCRYPT_BLOCK_LENGTH_IN_BYTE);
        //����һ���������
        for (n=0; n < ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++n)
        {
            src_buf[n] ^= plast_plain[n];
        }
        frame_xtea((int32_t*)src_buf, (int32_t*)ptmp_out, (int32_t*)key, DECRYPT_ROUNDS);
        //����һ���������
        for (n=0; n < ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++n)
        {
            ptmp_out[n] ^= plast_encrypt[n];
        }
        plast_encrypt = ptmp_in;
        plast_plain = ptmp_out;

        ptmp_in += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
        ptmp_out += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
    }

    //���8Byte�� �����7Byte��У��
    char last[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
    //����һ��8Byte�������
    memcpy(src_buf, ptmp_in, ENCRYPT_BLOCK_LENGTH_IN_BYTE);
    for (i=0; i < ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
    {
        src_buf[i] ^= plast_plain[i];
    }
    frame_xtea((int32_t*)src_buf, (int32_t*)last, (int32_t*)key, DECRYPT_ROUNDS);
    //����һ��8Byte�������
    for (i=0; i < ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
    {
        last[i] ^= plast_encrypt[i];
    }
    //У������0
    for (i=1; i < ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
    {
        if (last[i] != 0)
        {
            return -4;
        }
    }
    *ptmp_out = last[0];

    return plain_len;
}
