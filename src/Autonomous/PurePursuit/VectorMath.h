#pragma once

class Vector
{
    public:
        QLength x_component;
        QLength y_component;

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

        Vector(RawPoint point)
        {
            x_component = point.x_pos;
            y_component = point.y_pos;
        }

        Vector(PathPoint point)
        {
            x_component = point.x_pos;
            y_component = point.y_pos;   
        }

        Vector(RawPoint point_a, RawPoint point_b)
        {
            x_component = point_b.x_pos - point_a.x_pos;
            y_component = point_b.y_pos - point_a.y_pos; 
        }

        Vector(RobotPosition position)
        {
            x_component = position.x_pos;
            y_component = position.y_pos;
        }

        QLength magnitude()
        {
            double x_pow = std::pow((x_component.convert(inch)), 2);
            double y_pow = std::pow((y_component.convert(inch)), 2);
            double rooted = std::sqrt(x_pow + y_pow);
            QLength qlen = (rooted * inch);
            return qlen;
        }

        QAngle direction()
        {
            double theta = std::atan2(y_component.convert(inch), x_component.convert(inch)) * 180 / PI;
            double corrected = std::fmod((theta + 360), 360); 
            QAngle qang (corrected * degree);
            return qang;
        }

        Vector normalize()
        {
            Vector normalizedVector;
            QLength mag = magnitude();
            if(mag.convert(inch) != 0)
            { 
                normalizedVector.x_component =  QLength ((x_component.convert(inch) / mag.convert(inch)) * inch);
                normalizedVector.y_component =  QLength ((y_component.convert(inch) / mag.convert(inch)) * inch);
            }
            return normalizedVector;
        }

        Vector subtract(Vector subtrahend)
        {
            Vector resultantVector;
            resultantVector.x_component = QLength ((x_component.convert(inch) - subtrahend.x_component.convert(inch)) * inch);
            resultantVector.y_component = QLength ((y_component.convert(inch) - subtrahend.y_component.convert(inch)) * inch);
            return resultantVector;
        }

        Vector scalarMult(double scalar)
        {
            Vector scaledVector;
            scaledVector.x_component = QLength ((x_component.convert(inch) * scalar) * inch);
            scaledVector.y_component = QLength ((y_component.convert(inch) * scalar) * inch);
            return scaledVector;
        }

        QAngle angleBetween(Vector partner)
        {
            double angle = atan2(partner.y_component.convert(inch), partner.x_component.convert(inch)) - atan2(y_component.convert(inch), x_component.convert(inch));
            angle = std::abs(angle * 180 / PI);
            QAngle qang (angle * degree);
            return qang;
        }

        double dot(Vector multiplicand)
        {
            QAngle angle = angleBetween(multiplicand);
            return (magnitude().convert(inch) * multiplicand.magnitude().convert(inch)) * angle.convert(degree);
        }
};