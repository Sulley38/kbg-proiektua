/*
 * Kamera mota bat diren klaseen interfazea
 */

#ifndef ICAMERA_H
#define	ICAMERA_H

#include "Definitions.h"

class ICamera {
public:
    ICamera();
    virtual ~ICamera() {}
    
    // Kokapena lortzeko funtzioak
    const Array3& getEye() const { return eye; }
    const Array3& getAt() const { return at; }
    const Array3& getUp() const { return up; }
    
    // Puntu batera begira jarri
    void lookAt(const Array3& v) { at = v; }
    
    // Erlazio zenbakia eskuratu edo esleitu
    int getRelation() const { return relation; }
    void setRelation(const int rel) { relation = rel; }
    
    // Kamera mugitu
    virtual bool move(double x, double y, double z) = 0;
    // Kamera biratu
    virtual bool rotate(Axis axis, int angle) = 0;
    // Zoom egin
    virtual void zoom(double factor);
    
    // Proiekzio matrizea prestatu
    virtual void project(double aspectRatio);
    
    // Hasierako egoerara bueltatu
    virtual void reset() = 0;
    
protected:
    // Kameraren posizioa
    Array3 eye;
    // Kameraren orientazioa
    Array3 at;
    // Up bektorea
    Array3 up;
    // Bistaratze-eremuaren zabalera gradutan
    double fovy;
    // Erlazio-zenbakia
    int relation;
};

#endif	/* ICAMERA_H */