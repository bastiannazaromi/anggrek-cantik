<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class M_Rekap extends CI_Model {

	public function getAll()
	{
        $this->db->select('*');
        $this->db->from('tbdata');
        $this->db->order_by('id', 'desc');

        return $this->db->get()->result_array();
    }

    public function getGrafik()
	{
        $this->db->select('*');
        $this->db->from('tbdata');
        $this->db->order_by('id', 'desc');
        $this->db->limit(10);

        return $this->db->get()->result_array();
    }

    public function getLast()
    {
        $this->db->select('*');
        $this->db->from('tbdata');
        $this->db->order_by('id', 'desc');
        $this->db->limit(1);

        return $this->db->get()->result_array();
    }

    public function hapusRekap($id)
    {
        // $this->db->where('id', $id);
        $this->db->delete('tbdata', ['id' => $id]);
    }

}