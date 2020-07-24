<!-- Begin Page Content -->
<div class="container-fluid">

	<!-- Page Heading -->
	<h1 class="h3 mb-4 text-gray-800"><?= $title ; ?></h1>

    <div class="row">
        <div class="grafik col-md-12">
            <div class="card">
                <div class="card-header">
                
                </div>
                <div id="grafik" style="width:100%; height:480px;"></div>
            </div>
        </div>
    </div>

</div>

<script>
    var chart;
    var total=0;
    function tampil(){
            $.ajax({
                url: "<?php echo base_url('Dashboard/get_realtime') ; ?>",
                dataType:'json',
                success:function(result){
                    if (result.length>total){
                        total=result.length;
                        var i;
                        var suhu = [];
                        var kelembapan = [];
                        var cahaya = [];
                        var waktu = [];

                        for(i=0; i<result.length; i++){
                            suhu[i] = Number(result[i].suhu);
                            kelembapan[i] = Number(result[i].kelembapan);
                            cahaya[i] = Number(result[i].cahaya);
                            waktu[i] = result[i].waktu;
                            chart.series[0].setData(suhu);
                            chart.series[1].setData(kelembapan);
                            chart.series[2].setData(cahaya);
                            chart.xAxis[0].setCategories(waktu);
                        }
                        
                    }
                    else if (result.length<=total)
                    {
                        var i;
                        var suhu = [];
                        var kelembapan = [];
                        var cahaya = [];
                        var waktu = [];

                        for(i=0; i<result.length; i++){
                            suhu[i] = Number(result[i].suhu);
                            kelembapan[i] = Number(result[i].kelembapan);
                            cahaya[i] = Number(result[i].cahaya);
                            waktu[i] = result[i].waktu;
                            chart.series[0].setData(suhu);
                            chart.series[1].setData(kelembapan);
                            chart.series[2].setData(cahaya);
                            chart.xAxis[0].setCategories(waktu);
                        }
                        
                    }

                    setTimeout(tampil, 2000); 
                }
            });
    }
    
    document.addEventListener('DOMContentLoaded',function(){
        
        chart=Highcharts.chart('grafik',{
            chart:{
            type: 'line',
            events:{
                    load:tampil
                }
            },
            title:{
                text:'Data Rekap Monitoring Anggrek Cantik'
            },

            yAxis: {
                title: {
                    text: 'Nilai'
                }
            },

            xAxis: {
                
            },
            
            series:[{
                name:"Suhu"
            },
            {
                name:"Kelembapan"
            },
            {
                name:"cahaya"
            }]
        });
    });
</script>