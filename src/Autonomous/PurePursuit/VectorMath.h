#pragma once

template <typename T> int sgnum(T val) 
{
    return (T(0) < val) - (val < T(0));
}

class Vector
{
    public:
    
        QLength x_component;
        QLength y_component;

        Vector Vector::operator + (Vector addend) { return {x_component + addend.x_component, y_component + addend.y_component}; }

        Vector Vector::operator - (Vector subtrahend) { return {x_component - subtrahend.x_component, y_component - subtrahend.y_component}; }

        Vector Vector::operator * (double scalar) { return {x_component * scalar, y_component * scalar}; }

        Vector Vector::operator / (double scalar) { return {x_component / scalar, y_component / scalar}; }
        

        Vector()
        {
            x_component = 0_in;
            y_component = 0_in;
        }
        
        Vector(QLength x_comp, QLength y_comp)
        {
            x_component = x_comp;
            y_component = y_comp;
        }

        Vector(PathPoint point)
        {
            x_component = point.x_pos;
            y_component = point.y_pos;   
        }

        Vector(OdomState position)
        {
            x_component = position.x;
            y_component = position.y;
        }

        QLength magnitude()
        {
            double x_pow = std::pow((x_component.convert(meter)), 2);
            double y_pow = std::pow((y_component.convert(meter)), 2);
            double rooted = std::sqrt(x_pow + y_pow);
            QLength qlen = (rooted * meter);
            return qlen;
        }

        QAngle direction()
        {
            double theta = std::atan2(y_component.convert(meter), x_component.convert(meter)) * 180 / PI;
            double corrected = std::fmod((theta + 360), 360); 
            QAngle qang (corrected * degree);
            return qang;
        }

        Vector normalize()
        {
            Vector normalizedVector;
            QLength mag = magnitude();
            if(mag.convert(meter) != 0)
            { 
                normalizedVector.x_component =  QLength ((x_component.convert(meter) / mag.convert(meter)) * meter);
                normalizedVector.y_component =  QLength ((y_component.convert(meter) / mag.convert(meter)) * meter);
            }
            return normalizedVector;
        }

        double dot(Vector multiplicand)
        {
            return (x_component.convert(meter) * multiplicand.x_component.convert(meter)) + (y_component.convert(meter) * multiplicand.y_component.convert(meter));
        }
};

QLength interpointDistance(Vector point_a, Vector point_b)
{
    double x_components = std::pow(point_b.x_component.convert(meter) - point_a.x_component.convert(meter), 2);
    double y_components = std::pow(point_b.y_component.convert(meter) - point_a.y_component.convert(meter), 2);
    QLength qlen = ((std::sqrt(x_components + y_components)) * meter);
    return qlen;
}