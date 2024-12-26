// transformation.qml

import QtQuick 2.0

Item	{
    id: root_transformation
    //	set	width	based	on	given	background
    width:	parent.width
    height:	parent.height
    Image	{	//	nice	background	image
        id:	bg
        source:	"qrc:/images/background.jpg"
        anchors.fill: root_transformation
    }
    MouseArea	{
        id:	backgroundClicker
        //	needs	to	be	before	the	images	as	order	matters
        //	otherwise	this	mousearea	would	be	before	the	other	elements
        //	and	consume	the	mouse	events
        anchors.fill:	root_transformation
        onClicked:	{
            //	reset	our	little	scene
            star1.x	=	20
            star2.rotation	=	0
            star3.rotation	=	0
            star3.scale	=	1.0
        }
    }
    ClickableImage	{
        id:	star1
        x: 0; y: 150
        width: 100; height: 100
        source: "qrc:/images/star.png"
        onClicked: {
            //	increase	the	x-position	on	click
            x	+=	5
        }
    }
    ClickableImage	{
        id:	star2
        x: 400; y: 150
        width: 100; height: 100
        source: "qrc:/images/star.png"
        smooth:	true	//	need	antialising
        onClicked: {
            //	increase	the	rotation	on	click
            rotation	+=	5
        }
    }
    ClickableImage	{
        id:	star3
        x: 600; y: 150
        width: 100; height: 100
        source: "qrc:/images/star.png"
        smooth:	true	//	need	antialising
        onClicked: {
            //	several	transformations
            rotation	+=	5
            scale	-=	0.05
        }
    }
}
