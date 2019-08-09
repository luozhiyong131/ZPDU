/*
 * cfg_op.c
 *
 *  Created on: 2019年8月8日
 *      Author: Lzy
 */

#define  _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MaxLine 2048

//获取配置项
int cfg_item_read(const char *pFileName /*in*/, const char *pKey /*in*/, char * pValue/*in out*/, int * pValueLen /*out*/)
{
    int        ret = 0;
    FILE    *fp = NULL;
    char    *pTmp = NULL, *pEnd = NULL, *pBegin = NULL;

    char lineBuf[MaxLine];

    fp = fopen(pFileName, "r");//读的方式打开文件
    if (fp == NULL)
    {
        ret = -1;
        return ret;
    }

    while (!feof(fp))//没有到达文件末尾
    {
        memset(lineBuf, 0, sizeof(lineBuf));//清空内存空间
         //fgets(_Out_z_cap_(_MaxCount) char * _Buf, _In_ int _MaxCount, _Inout_ FILE * _File);
        fgets(lineBuf, MaxLine, fp);//从文件中一行一行的获取数据
        //printf("lineBuf:%s ",lineBuf );

        pTmp = strchr(lineBuf, '='); //有没有等号
        if (pTmp == NULL) //没有=号
        {
            continue;
        }

        pTmp = strstr(lineBuf, pKey);//所在行 是不是有key
        if (pTmp == NULL) //判断key是不是在
        {
            continue;
        }
        pTmp = pTmp + strlen(pKey); //mykey1 = myvalude11111111 ==> "= myvalude1111111"

        pTmp = strchr(pTmp, '=');
        if (pTmp == NULL) //判断key是不是在 //所在行 是不是有key
        {
            continue;
        }
        pTmp = pTmp + 1;
        //
        //printf("pTmp:%s ", pTmp);

        //获取value 起点
        while (1)
        {
            if (*pTmp == ' ')
            {
                pTmp ++ ;
            }
            else
            {
                pBegin = pTmp;
                if (*pBegin == '\n')
                {
                    //没有配置value
                    //printf("配置项:%s 没有配置value \n", pKey);
                    goto End;
                }
                break;
            }
        }

        //获取valude结束点
        while (1)
        {
            if ((*pTmp == ' ' || *pTmp == '\n'))
            {
                break;
            }
            else
            {
                pTmp ++;
            }
        }
        pEnd = pTmp;

        //赋值
        *pValueLen = pEnd-pBegin;
        memcpy(pValue, pBegin, pEnd-pBegin);
    }

End:
    if (fp == NULL)
    {
        fclose(fp);
    }
    return 0;

}

//写配置项
//实现流程
//循环读每一行，检查key配置项是否存在 若存在修改对应value值
//若不存在，在文件末尾 添加 "key = value"
//难点：如何修改文件流中的值
int cfg_item_write(const char *pFileName /*in*/, const char *pKey /*in*/, const char * pValue/*in*/)
{
    int        rv = 0, iTag = 0, length = 0;
    FILE    *fp = NULL;
    char    lineBuf[MaxLine];
    char    *pTmp = NULL;
    char    filebuf[1024*8] = {0};

    if (pFileName==NULL || pKey==NULL || pValue==NULL)
    {
        rv = -1;
        printf("SetCfgItem() err. param err \n");
        goto End;
    }

    fp = fopen(pFileName, "r+");
    if (fp == NULL)
    {
        rv = -2;
        printf("fopen() err. \n");
        //goto End;
    }

    if (fp == NULL)//文件不存在就去创建一个文件
    {
        fp = fopen(pFileName, "w+t");
        if (fp == NULL)
        {
            rv = -3;
            printf("fopen() err. \n");
            goto End;
        }
    }

    fseek(fp, 0L, SEEK_END); //刚开始文件指针指向的是文件的开头，把文件指针从0位置开始，移动到文件末尾
    //获取文件长度;
    length = ftell(fp);//fp指向了文件的末尾

    fseek(fp, 0L, SEEK_SET);//再把文件指针指向文件的头部

    if (length > 1024*8) //文件不能超过8K,不然内存不支持
    {
        rv = -3;
        printf("文件超过1024*8, nunsupport");
        goto End;
    }

    while (!feof(fp))
    {
        //读每一行
        memset(lineBuf, 0, sizeof(lineBuf));
        pTmp = fgets(lineBuf, MaxLine, fp);
        if (pTmp == NULL)
        {
            break;
        }

        //key关键字是否在本行
        pTmp = strstr(lineBuf, pKey);
        if (pTmp == NULL) //key关键字不在本行， copy到filebuf中
        {
            strcat(filebuf, lineBuf);//复制到filebuf中去
            continue;
        }
        else  //key关键在在本行中，替换旧的行，再copy到filebuf中
        {
            sprintf(lineBuf, "%s = %s\n", pKey, pValue);
            strcat(filebuf, lineBuf);
            //若存在key
            iTag = 1;
        }
    }

    //所有的行中若key关键字，不存在 追加
    if (iTag == 0)
    {
        fprintf(fp, "%s = %s\n", pKey, pValue);//格式化输入
    }
    else //若key关键字，存在，则重新创建文件
    {
        if (fp != NULL)
        {
            fclose(fp); //关闭文件
            fp = NULL; //避免野指针
        }
        fp = fopen(pFileName, "w+t"); //重新建立一个文件
        if (fp == NULL)
        {
            rv = -4;
            printf("fopen() err. \n");
            goto End;
        }
        fputs(filebuf, fp);//把所有文件缓冲的内容输入到fp，覆盖原来的文件
        //fwrite(filebuf, sizeof(char), strlen(filebuf), fp);
    }

End:
    if (fp != NULL)
    {
        fclose(fp);
    }
    return rv;
}
