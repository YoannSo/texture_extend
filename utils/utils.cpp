int smallest(int x, int y, int z){
    return min(min(x, y), z);
}

int getOfsetOfMin(int x,int y, int z){
    if(x<y){
        if(x<z){
            return -1;
        }
        if(y<z){
            return 0;
        }
        return 1;
    }
    if(y<z)
        return 0;
    return 1;
}