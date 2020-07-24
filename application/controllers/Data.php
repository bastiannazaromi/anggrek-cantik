<?php
defined('BASEPATH') or exit('No direct script access allowed');

class Data extends CI_Controller
{

    public function save()
    {
        $this->load->model('M_Data', 'data');

        $suhu = $this->input->get('suhu', TRUE);
        $kelembapan = $this->input->get('kelembapan', TRUE);
        $cahaya = $this->input->get('cahaya', TRUE);
        $debit = $this->input->get('debit', TRUE);
        $volume = $this->input->get('volume', TRUE);

        // data dari M_Simpan.php
        $rekap = $this->data->ambil_data_terakhir();

        if ($rekap) {
            $suhu_s = $rekap[0]["suhu"];
            $kelembapan_s = $rekap[0]["kelembapan"];
            $cahaya_s = $rekap[0]["cahaya"];
            $debit_s = $rekap[0]["debit"];
            $volume_s = $rekap[0]["volume"];

            $awal  = date_create($rekap[0]['waktu']);
            $akhir = date_create(); // waktu sekarang
            $diff  = date_diff($awal, $akhir);

            $hari = $diff->d;
            $jam = $diff->h;

            if ($suhu_s == $suhu && $kelembapan_s == $kelembapan && $cahaya_s == $cahaya && $debit_s == $debit && $volume_s == $volume) {
                if ($hari >= 1 || $jam >= 1) {
                    // Simoan ke database
                    $this->data->save($suhu, $kelembapan, $cahaya, $debit, $volume);
                    echo "Sukses, data tersimpan di database";
                } else {
                    echo "Sukses, data masih sama";
                }
            } else {
                // Simpan ke database
                $this->data->save($suhu, $kelembapan, $cahaya, $debit, $volume);
                echo "Sukses, data tersimpan di database";
            }
        } else {
            // Simpan ke database
            $this->data->save($suhu, $kelembapan, $cahaya, $debit, $volume);
            echo "Sukses, data tersimpan di database";
        }
    }

    public function data_terakhir()
    {
        $this->load->model('M_Data', 'data');

        $rekap = $this->data->ambil_data_terakhir();

        $cahaya = $rekap[0]["cahaya"];

        if ($cahaya >= 30) {
            echo "false";
        } else {
            echo "true";
        }
    }
}
