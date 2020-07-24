<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class M_Data extends CI_Model {

	public function save($suhu, $kelembapan, $cahaya, $debit, $volume)
	{
        $tanggal = date('Y-m-d H:i:s');
        
        $data = [
            "waktu" => $tanggal,
            "suhu" => $suhu,
            "kelembapan" => $kelembapan,
            "cahaya" => $cahaya,
            "debit" => $debit,
            "volume" => $volume
        ];

        $this->db->insert('tbdata', $data);
    }

    public function ambil_data_terakhir()
    {
        $this->db->select('*');
        $this->db->from('tbdata');
        $this->db->order_by('id', 'desc');
        $this->db->limit(1);

        return $this->db->get()->result_array();
    }

}