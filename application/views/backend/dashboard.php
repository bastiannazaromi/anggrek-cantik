<!-- Begin Page Content -->
<div class="container-fluid">

  <!-- Page Heading -->
  <h1 class="h3 mb-4 text-gray-800"><?= $title ; ?></h1>

  <div class="row">
    
    <div class="col-xl-6 col-md-6 mb-4">
      <div class="card border-left-primary shadow h-100 py-2">
        <div class="card-body">
          <div class="row no-gutters align-items-center">
            <div class="col mr-2">
              <div class="text-xs font-weight-bold text-primary text-uppercase mb-2">Suhu</div>
              <div class="h5 mb-0 font-weight-bold text-gray-800" id="suhu"></div>
            </div>
            <div class="col-auto">
              <i class="fas fa-temperature-low fa-2x"></i>
            </div>
          </div>
        </div>
      </div>
    </div>

    <div class="col-xl-6 col-md-6 mb-4">
      <div class="card border-left-primary shadow h-100 py-2">
        <div class="card-body">
          <div class="row no-gutters align-items-center">
            <div class="col mr-2">
              <div class="text-xs font-weight-bold text-primary text-uppercase mb-2">Kelembapan</div>
              <div class="h5 mb-0 font-weight-bold text-gray-800" id="kelembapan"></div>
            </div>
            <div class="col-auto">
              <i class="fas fa-temperature-high fa-2x"></i>
            </div>
          </div>
        </div>
      </div>
    </div>

    <div class="col-xl-6 col-md-6 mb-4">
      <div class="card border-left-primary shadow h-100 py-2">
        <div class="card-body">
          <div class="row no-gutters align-items-center">
            <div class="col mr-2">
              <div class="text-xs font-weight-bold text-primary text-uppercase mb-2">Cahaya</div>
              <div class="h5 mb-0 font-weight-bold text-gray-800" id="cahaya"></div>
            </div>
            <div class="col-auto">
              <i class="fas fa-sun fa-2x"></i>
            </div>
          </div>
        </div>
      </div>
    </div>

    <div class="col-xl-6 col-md-6 mb-4">
      <div class="card border-left-primary shadow h-100 py-2">
        <div class="card-body">
          <div class="row no-gutters align-items-center">
            <div class="col mr-2">
              <div class="text-xs font-weight-bold text-primary text-uppercase mb-2">Total Air</div>
              <div class="h5 mb-0 font-weight-bold text-gray-800" id="total"></div>
            </div>
            <div class="col-auto">
              <i class="fas fa-water fa-2x"></i>
            </div>
          </div>
        </div>
      </div>
    </div>

  </div>

</div>

<script>

  function tampil(){
    $.ajax({
      url: "<?= base_url('Dashboard/dashboard_realtime')?>",
      dataType: 'json',
      success:function(result){
        
        $('#suhu').text(result[0]["suhu"]);
        $('#suhu').append('<sup style="font-size: 20px">o</sup>');
        
        $('#kelembapan').text(result[0]["kelembapan"] + " %");
        $('#cahaya').text(result[0]["cahaya"] + " %");
        $('#total').text(result[0]["volume"] + " mL");
        setTimeout(tampil, 2000); 
      }
    });
  }
  
  document.addEventListener('DOMContentLoaded',function(){
    tampil();
  });

</script>
