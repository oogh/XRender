//
// Created by Oogh on 2020/3/19.
//

#ifndef XPRODUCABLE_H
#define XPRODUCABLE_H

class XProducable {
public:
    XProducable();
    
    virtual ~XProducable();
    
    virtual void setInput(const char* filename);
    
    virtual void start();
    
//    virtual void fillVideoData(uint8_t* pixels[4], int width, int height);
    
};

#endif //XPRODUCABLE_H
