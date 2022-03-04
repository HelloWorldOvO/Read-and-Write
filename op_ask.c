#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#define MAXINPUT 100
#define FILE "./test.txt"
#define READ_LEN 1032

void ERROR_DEAL(int error){
    switch (error)
    {
    case 1/* input more than three argument */:
        printf("more than three argument\n");
        break;
    case 2/* operator more thar one char */:
        printf("you can just input operator r (read) or w (write)\n");
        break;
    case 3/* file open error */:
        printf("file open error\n");
        break;
    case 4/* pos and size error */:
        printf("Argument pos and size can't be less than 0, and size can't be equal to 0\n");
        break;
    case 5/* part over the file */:
        printf("the part to operate of the file is over the file\n");
        break;
    case 6:
        break;
    

    default:
        break;
    }

    return;
}

void remove_space(char *array,int arrlen){
    // The final array formate is one space between the near two non-space block
    int i,j;
    for(i=0;i<arrlen-1;i++){
        if((array[i] == ' ' && array[i+1] == ' ') || array[0] == ' '){ // for the 0 at begin or two beside 0
            for(j=i;j<arrlen;j++){ // all element move front one
                array[j] = array[j+1];
            }
            array[arrlen-1] = '\0';
            i--;
            arrlen--;
        }
        if(array[arrlen-1] == ' '){
            array[arrlen-1] = '\0';
            arrlen--;
        }
    }
}

int deal_three_input(char *input_str, int str_length, char *foperator, int *fpos, int *fsize){
    // Translate input command str to all operator
    // Check if the input format is wrong 

    int error=0;
    int space_num = 0;
    int space1=-1, space2=-1;
    char str_pos[MAXINPUT] = "";
    char str_size[MAXINPUT] = "";
    char temp[2] = "";temp[1] = '\0';
        
    int i,j;
    for(i=0;i<str_length;i++){
        if(input_str[i] == ' '){
            space_num++;
            if(space1 == -1){
                space1 = i;
            }else if(space2 == -1){
                space2 = i;
            }
        }
    }
    printf("cml:%s\n",input_str);

    if(space_num > 2){
        error = 1; // more than three argument
        return error;
    }else if(space1 != 1){
        error = 2; // operator more thar one char
        return error;
    }

    *foperator = input_str[0]; // set operator by the first char in input command
    if(!(*foperator == 'r' || *foperator == 'w')){ // check if operator is wrong
        error = 2;
        return error;
    }

    for(i=space1+1;i<space2;i++){ // get the pos str
        temp[0] = input_str[i];
        strcat(str_pos,temp);
    }

    *fpos = atoi(str_pos); // sranslate pos str to int

    for(i=space2+1;i<str_length;i++){ // get the pos str
        temp[0] = input_str[i];
        strcat(str_size,temp);
    }
    *fsize = atoi(str_size);
    return error;
}

int read_op(int fpos, int fsize){
    // Read file
    int error = 0;
    int file_len = 0; // file size
    int read_len = 0;
    int read_end = fpos + fsize;
    int remain_size = fsize; // how many data shall read
    char buf[READ_LEN] = {0};


    int read_fd = open(FILE,O_RDWR);

    if(fpos < 0 || fsize <= 0){ // pos and size error
        error = 4;
        return error;
    }

    if(read_fd < 0){ // file open error
        error = 3;
        return error;
    }


    file_len = (int)lseek(read_fd,0,SEEK_END);//get file total size

    if(file_len < read_end){ // read part over the file
        error = 5;
        return error;
    }

    // printf("file_total:%d\n",file_total);
    lseek(read_fd,fpos-1,SEEK_SET); // move cursor to the front one of target char
    /* read start */
    while(remain_size > 0){ // if something need to be read
        if(remain_size < READ_LEN){ // if data need to read less than a block
            read_len = read(read_fd,buf,remain_size);
        }else{  // if more than the block's data need to read
            read_len = read(read_fd,buf,READ_LEN);
        }
        
        printf("%s",buf);
        remain_size -= read_len;
    }
    printf("\n");
    close(read_fd);
    return error;
}

int main(){
    int error_num = 0;
    char input_line[MAXINPUT] = "";
    char *input = input_line;
    int input_length;
    char operator;
    int pos,size;

    while(1){
        printf("input : [r/w] [pos] [size] \n");
        scanf ("%[^\n]%*c", input_line); //input command
        input_length = strlen(input_line); // get length of input

        remove_space(input,input_length); //remove input's space
        input_length = strlen(input_line);

        error_num = deal_three_input(input, input_length, &operator, &pos, &size); // put all value to all operator
        memset(&input_line[0], 0, sizeof(input_line));
        if(error_num != 0){
            ERROR_DEAL(error_num);
            error_num = 0;
            continue;
        }

        if(operator == 'r'){ // if need to read file
            error_num = read_op(pos, size);
            if(error_num != 0){
                ERROR_DEAL(error_num);
                error_num = 0;
                continue;
            }
        }
        
        

    }

    return 0;
}

