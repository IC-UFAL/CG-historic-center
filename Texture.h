#ifndef HISTORIC_CENTER_TEXTURE_H
#define HISTORIC_CENTER_TEXTURE_H


GLuint loadTexture(const char *filename, int width, int height) {
    GLuint texture;
    unsigned char *data;
    FILE *file;

    //The following code will read in our RAW file
    file = fopen(filename, "rb");
    if (file == NULL) return 0;
    data = (unsigned char *) malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);
    fclose(file);

    for (int i = 0; i < width * height; ++i) {
        int index = i * 3;
        unsigned char B, R;
        B = data[index];
        R = data[index + 2];

        data[index] = R;
        data[index + 2] = B;

    }

    glGenTextures(1, &texture); //generate the texture with the loaded data
    glBindTexture(GL_TEXTURE_2D, texture); //bind the texture to it’s array
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);
    return texture; //return whether it was successfull
}

void FreeTexture(GLuint texture) {
    glDeleteTextures(1, &texture);
}

#endif //HISTORIC_CENTER_TEXTURE_H
