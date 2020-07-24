<?php
defined('BASEPATH') OR exit('No direct script access allowed');
class M_login extends CI_Model {
	function cek_login($u,$p){
		// $data		= $this->db->get_where('user',array('email_user' => $u,'password_user' => MD5($p)))->result();
		$this->db->where('email', $u);
		$a=$this->db->get('tbuser');
		$data = $a->result();
		if (count($data) === 1) {
			if(password_verify($p, $data[0]->password))
			{
				$login		=	array(
				'is_logged_in'	=> 	true,
				'email'			=>	$u,
				'id'			=>	$data[0]->id,
				'nama'			=>	$data[0]->nama,
				'foto'			=>  $data[0]->foto,
				'date_created'	=>  $data[0]->created
				);
				if ($login) {
					$this->session->set_userdata( 'data_login' , $login );
					$this->session->set_userdata($login);
					return 'Valid';
				}
			}
			else
			{
				return 'Password Salah';
			}
		}
		else
		{
			return 'Email tidak terdaftar';
		}
		
	}
}
/* End of file M_login.php */
/* Location: ./application/models/M_login.php */