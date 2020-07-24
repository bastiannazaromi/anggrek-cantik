<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Auth extends CI_Controller {

	public function __construct()
	{
		parent::__construct();
		if (!empty($this->session->userdata('data_login'))) {
			if ($this->uri->segment(2) != 'logout')
			{
				redirect('Dashboard');
			}
		}
	}

	public function index()
	{
		
		$data['title'] = 'Halaman Login';
		$data['page'] = 'auth/login';
		$this->load->view('auth/index', $data, FALSE);
		
	}

	public function login()
	{
		$email = $this->input->post("email");
		$password = $this->input->post("password");

		$this->load->model('M_login');
		$a = $this->M_login->cek_login($email,$password);
		
		echo $a;
	}

	public function logout(){
		$this->session->sess_destroy($this->session->userdata('data_login'));
		redirect('Auth', 'refresh');
	}
	
}

/* End of file Auth.php */
/* Location: ./application/controllers/Auth.php */