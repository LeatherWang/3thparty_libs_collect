#include "ticppapi.h"
#include "ticpp.h"

using namespace ticpp;

int main(int argc, char** argv)
{
//    Node* root(doc.InsertEndChild(Element("robot name=\"eai_2st\"")));
//    Element joint("joint");
//    Element ele1("parent",  12);
//    Element ele2("children");
//    Element ele3("origin");
//    ele3.SetAttribute("rpy", "0 0 0");
//    joint.InsertEndChild(ele1);
//    joint.InsertEndChild(ele2);
//    joint.InsertEndChild(ele3);
//    root->InsertEndChild(measurements);

//    ticpp::Element* pElem = (ticpp::Element*)doc.FirstChildElement()->NextSibling();

//    int attr2;
//    pElem->GetAttribute( "myAttribute", &attr2 );
//    std::cout << child->GetAttribute("rpy") << ", " << *out << ", " << child->GetText(false) << std::endl;

    Document doc;
    doc.LoadFile("/home/leather/ros_all/dashgo_ws/src/dashgo/dashgo_description/urdf/dashgobase/base.urdf.xacro");

    ticpp::Element* root = doc.FirstChildElement(true);
    ticpp::Element* child = root->FirstChildElement(false);

    std::string* p_value = new std::string();
    for(; child; child = child->NextSiblingElement(false)) {
        child->GetValue(p_value);
        if(*p_value == "joint") {
            if(child->GetAttribute("name") == "laser_frame_joint") {
                ticpp::Element* joint = child->FirstChildElement(false );
                for(; joint; joint = joint->NextSiblingElement(false)) {
                    p_value->clear();
                    joint->GetValue(p_value);
                    if(*p_value == "origin") {
                        joint->SetAttribute("rpy", "5.0 0 2");
                    }
                }
            }
        }

        if(*p_value == "xacro:property") {
            std::cout << child->GetAttribute("name") << std::endl;
        }
    }

    delete p_value;

    doc.SaveFile("./test.xml");

    return 0;
}
