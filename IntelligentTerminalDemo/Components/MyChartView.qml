import QtQuick 2.0
import QtQuick.Window 2.0
import QtCharts 2.2

Item {
    id :root_chart
    anchors.fill: parent
    property ListModel points: ListModel {}
    property string showType: "lp" //lp sp
    property bool showPoint: true
    property string xTitle: ""
    property string yTitle: ""
    property int xMin: 0
    ChartView {
        id: chartView
        // title: "折线图示例"
        antialiasing: true // 抗锯齿
        // backgroundColor: Qt.rgba(100, 149, 237, 0.1) // transparent
        animationOptions: ChartView.SeriesAnimations
        animationDuration: 1
        anchors.fill: parent
        ValueAxis { // X轴
            id: x_axis
            // max: points.count
            // min: 1
            // tickCount: points.count
            // minorTickCount: 0 //没有设置次要刻度线。
            // minorGridLineColor: "red" // 设置次要网格线的颜色
            // color: "red" // 设置轴线的颜色
            // 设置刻度的格式
            labelFormat: "%i" // "07." + "%02.0f"
            // labelsColor: "red" // 设置轴上刻度线的值的颜色
            titleText: xTitle
            // labelsFont.pixelSize: 24
            // labelsFont.family: "Microsoft YaHei"
            // titleVisible: true
        }
        ValueAxis { // Y轴
            id: y_axis
            // max: 50 // 设置最大值
            // min: 0 // 设置最小值
            // tickCount: 10 // 设置网格线条数
            labelFormat: "%i"
            titleText: yTitle // 设置单位
            //titleFont.family: "Microsoft YaHei" // 设置单位字体
            //titleFont.pixelSize: 24
            //labelsFont.family: "Microsoft YaHei" // 设置Y轴值字体
            //labelsFont.pixelSize: 24
        }

        SplineSeries{
            id:splineSeries
            color: Qt.rgba(255,0,0,1)
            axisX: x_axis
            axisY: y_axis
        }
        LineSeries {
            id: lineSeries
            axisX: x_axis
            axisY: y_axis
            style: Qt.SolidLine
            // axisYRight: y_axis // Y轴展示在折线图右侧(默认为左侧)
            // axisXTop: x_axis // X轴展示在折线图上方(默认为下方)
            color: "skyblue" // 设置折线颜色
            width: 3 // 设置折线宽
        }
        ScatterSeries {
            id: pointSeries
            axisX: x_axis
            axisY: y_axis
            markerSize: 7  // 设置markerSize为0，即隐藏圆点
            markerShape: ScatterSeries.MarkerShapeCircle // 设置圆点形状
            color: "#6495ED"
            pointLabelsFormat: "@yPoint"
            pointLabelsVisible: true
            pointLabelsColor: "#6495ED"
            pointLabelsClipping: false
        }
        Component.onCompleted: {
            chartView.legend.visible = false
            var maxX = 0
            var maxY = 0
            for (var i = 0; i < points.count; i++) {
                var item = points.get(i)
                switch (showType) {
                case "lp":
                    lineSeries.append(item.x, item.y)
                    break;
                case "sp":
                    splineSeries.append(item.x, item.y)
                    break;
                }
                if (showPoint) {
                    pointSeries.append(item.x, item.y)
                }
                maxX = item.x > maxX ? item.x : maxX
                maxY = item.y > maxY ? item.y : maxY
            }
            y_axis.max = maxY
            x_axis.max = maxX
            x_axis.tickCount = maxX
            x_axis.min = xMin
        }
    }
}
