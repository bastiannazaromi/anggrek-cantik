<!-- Begin Page Content -->
<div class="container-fluid">

	<!-- Page Heading -->
	<h1 class="h3 mb-4 text-gray-800"><?= $title ; ?></h1>

    <div class="row">
	    <div class="col-xl-12 col-md-12 mb-4">
            <div class="card">
                <div class="card-body">
                <div class="table-responsive">
                    <table id="example" class="table table-bordered table-hover">
                        <thead class="bg-light text-dark">
                        <tr>
                            <th>#</th>
                            <th>Waktu</th>
                            <th>Suhu</th>
                            <th>Kelembapan</th>
                            <th>Cahaya</th>
                            <th>Debit Air</th>
                            <th>Total Air</th>
                            <th>Action</th>
                        </tr>
                        </thead>
                        <tbody>
                            <?php $i = 1;
                            foreach ($suhu as $hasil) : ?>
                                <tr>
                                    <th><?= $i++ ?></th>
                                    <td><?= date('d F Y - H:i:s', strtotime($hasil['waktu'])) ; ?></td>
                                    <td><?= $hasil['suhu'] . '<sup style="font-size: 10px">o</sup>' ; ?></td>
                                    <td><?= $hasil['kelembapan'] . " %" ; ?></td>
                                    <td><?= $hasil['cahaya'] . " %" ; ?></td>
                                    <td><?= $hasil['debit'] . " L/min" ; ?></td>
                                    <td><?= $hasil['volume'] . " mL" ; ?></td>
                                    <td>
                                        <a href="<?= base_url() ?>Dashboard/hapusRekap/<?= $hasil['id']; ?>" class="badge badge-danger delete-people tombol-hapus"><i class="fa fa-trash"></i> Hapus</a>
                                    </td>
                                </tr>
                            <?php endforeach; ?>
                        </tbody>
                    </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>