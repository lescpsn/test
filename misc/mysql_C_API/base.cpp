

BOOL Init()
{
	int ret = 0;
	int ConnectTimeout = 2;
	mysql = mysql_init(NULL);//��ʼ��;
	if (!mysql)
	{
		printf("mysql_init failed!\n");
		return FALSE;
	}
	ret = mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, (const char*)&ConnectTimeout);//���ó�ʱѡ��
	if (ret)
	{
		printf("Options Set ERRO!\n");
		free (mysql);
		return FALSE;
	}
	mysql = mysql_real_connect(mysql, MYSQLADDR, MYSQLUSER, MYSQLPW, DATABASENAME, 0, NULL, 0);//����MySQL testdb���ݿ�
	if (mysql)
	{
		printf("Connection Succeed!\n");
		return TRUE;
	}
	else    //������
	{
		printf("Connection Failed!\n");
		if (mysql_errno(mysql))
		{
			printf("Connect Erro:%d %s\n", mysql_errno(mysql), mysql_error(mysql));//���ش�����롢������Ϣ
		}
		return FALSE;
	}
}

void Exit()
{
	if (mysql != NULL)
	{
		mysql_close(mysql);
		//free(mysql);
	}
}