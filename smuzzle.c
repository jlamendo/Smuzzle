#define _GNU_SOURCE
#define MAXLINES 500;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
char path[]="YOUR FILEPATH GOES HERE";

void errorz(){
  char err[]="Error on fread! Aborting!";
  printf("%s", err);
  exit(0);
}

void gen_fuzziness(char *dest) {
    size_t length = rand() % 30;
    if(length<5){ //fast and lazy way to prevent numbers 1-5
            length=length+5;
            }
    char charset[] = "0123456789"
                     "{}[]()';%#$@^*`!<>/;:,.?|+=-_";
               //    "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";          //additional charsets can go here

    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

void fuzzFile(char fuzztag[], int i){

 FILE *filePointer;
 char fuzziness[100]= "null";
 char filename[]="AAAAAAAAAAAAAAAAAAAA";
 char tmp_var[20]="null";
 char tmp_i[20]="null";
 char prefixbuf1[] = "<!doctype html><meta http-equiv=\"refresh\" content=\"1; url=file:///";
 char prefixbuf2[] = ".html/\">\n<html>\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/xhtml;charset=utf-8\"/>\n<title>innerHTML Test-Tool</title>\n<style>\n#canvas, #canvas2, #html, #log\n{width:100%;height:175px;overflow-y:scroll}\niframe#iframe\n{display:none}\n</style>\n</head>\n<body>\n<textarea id=\"html\" ><div style=\"";

 char suffbuf[]= "onError=alert(\"Great Success!!!\");\">123</textarea>\n<hr />\n<div id=\"canvas\"></div>\n<hr />\n<button onclick=\"fillIframe()\">document.write(innerHTML)</button>\n<button onclick=\"applyCssText()\">Apply style.cssText()</button>\n<hr />\n<textarea id=\"log\"></textarea>\n<hr/>\n<div id=\"canvas2\"></div>\n<iframe id=\"iframe\" src=\"#\"></iframe>\n<script>\nvar html = document.getElementById('html');\nvar log = document.getElementById('log');\nvar canvas = document.getElementById('canvas');\nvar canvas2=document.getElementById('canvas2');\nvar iframe = document.getElementById('iframe');\nlocation.hash.length > 1 ? html.value=location.hash.slice(1) : false; \nlocation.hash = '';\nvar updateCanvas = function(e){\nif(e && e.keyCode && (e.keyCode == 27 || e.keyCode == 37 || e.keyCode == 38 || e.keyCode == 39 || e.keyCode == 40)) {\nreturn false;\n}\ncanvas.innerHTML=html.value\nlog.value=canvas.innerHTML;\ncanvas2.innerHTML=canvas.innerHTML;\nlocation.hash=html.value; // a little document.write() backup\ntry {\nlog.value += '\\r\\n\\r\\n=== Additional Info ===\\r\\nonclick:' + canvas.firstChild.onclick;\nlog.value += '\\r\\nvalue: ' + canvas.firstChild.value;\nlog.value += '\\r\\nstyle.cssText: ' + canvas.firstChild.style.cssText;\nlog.value += '\\r\\ncurrentStyle.cssText: ' + canvas.firstChild.currentStyle.cssText;\nlog.value += '\\r\\nruntimeStyle.cssText: ' + canvas.firstChild.runtimeStyle.cssText;\n} catch(e) {e}\n};\nvar fillIframe = function(){\niframe.contentWindow.document.write(canvas.innerHTML);\niframe.contentWindow.document.close();\n}\nvar applyCssText = function(){\ncanvas2.firstChild.style.cssText = canvas.firstChild.style.cssText;\n}\nhtml.onkeyup = updateCanvas;\nwindow.onload = updateCanvas;\n</script>\n</body>\n</html>";

 
 gen_fuzziness(fuzziness); //put some hair on them peaches
 snprintf(&tmp_var, sizeof(tmp_var), "%d", i);
 i++;
 snprintf(&tmp_i, sizeof(tmp_i), "%d", i);
 strncat (tmp_var, ".html", 20); 
 filePointer = fopen(tmp_var, "w");
if (filePointer == NULL) {
  fprintf(stderr, "Can't open output file %s.html!\n", fuzztag);
  exit(1);
 }


 fprintf(filePointer, "%s%s%s%s%s:%s%s", prefixbuf1, path, tmp_i, prefixbuf2, fuzztag, fuzziness, suffbuf); 
}

int main(int argc, char **argv){
  int i=0;
  if(argc<2){
    i=0;//do nothing;
  } else {
    i = atoi(argv[1]);
  }
srand(time(NULL));
char *config = NULL;
FILE *fp = fopen("fuzz.txt", "r");
char * line = NULL;
size_t len = 0;
ssize_t read;



while ((read = getline(&line, &len, fp)) != -1) {
           len = strlen(line);
           if( line[len-1] == '\n' ){
           line[len-1] = 0;
           }
	   fuzzFile(line, i);
	   i++;

       }

       if (line)
           free(line);
       printf("Great Success! %d fuzz files created. Use ./fuzz.sh to create %d more!\n",i,i);
       exit(EXIT_SUCCESS);
   }






