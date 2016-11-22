#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <iostream>

bool getUrlToFile(const char *filename)
{
	std::cout << "---- getUrlToFile ----" << std::endl;
    CURL *curl;
    CURLcode res;
    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL) // save by file
        return false;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: Agent-007");
    curl = curl_easy_init(); // init
    if (curl)
    {
        //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080"); // proxy
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); // set header
        // curl_easy_setopt(curl, CURLOPT_URL,"http://www.baidu.com");
        curl_easy_setopt(curl, CURLOPT_URL,"https://openapi.360.cn/user/me.json?access_token=12345678983b38aabcdef387453ac8133ac3263987654321&fields=id,name,avatar,sex,area"); // set url
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp); // save return header to file
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); // save return content to file

        res = curl_easy_perform(curl); // excute
        if (res != 0) {

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }
        fclose(fp);
        return true;
    }
	return false;
}

size_t getContent(void* ptr, size_t size, size_t nmemb, void* stream)
{
	std::string *pBuffer = (std::string *)stream;
	size_t data_size = size*nmemb;
    if (pBuffer != NULL){
        pBuffer->append((char *)ptr,data_size);
    }
	return data_size;
}

bool getUrlToString()
{
	std::cout << "---- getUrlToString ----" << std::endl;
	std::string buffer = "";
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: Agent-007");
    curl = curl_easy_init(); // init
    if (!curl)
    {
		return false;
	}
	//curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080"); // proxy
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); // set header
	curl_easy_setopt(curl, CURLOPT_URL,"https://openapi.360.cn/user/me.json?access_token=12345678983b38aabcdef387453ac8133ac3263987654321&fields=id,name,avatar,sex,area"); // set url
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getContent); // callback function
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer); // callback function param

	res = curl_easy_perform(curl);   // excute
	if (res != 0) {
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	}

	std::cout << "buffer=" << buffer << std::endl;
	return true;
}

bool postUrl(const char *filename)
{
	std::cout << "---- postUrl ----" << std::endl;
    CURL *curl;
    CURLcode res;
    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL)
        return false;
    curl = curl_easy_init();
    if (curl)
    {
        // curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cookie.txt"); // set cookie file
        // curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080"); // set proxy
        curl_easy_setopt(curl, CURLOPT_URL, " http://mail.sina.com.cn/cgi-bin/login.cgi "); // set url
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "&logintype=uid&u=xieyan&psw=xxx86"); // set post data
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    fclose(fp);
    return true;
}

int main(void)
{
    // getUrlToFile("./get.html");
    getUrlToString();
    // postUrl("./post.html");
	return 0;
}
