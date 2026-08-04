public class Livro {
    private String titulo;
    private String autor;
    private int anoPublicacao;

    public Livro() {
        this.titulo = "Sem título";
        this.autor = "Desconhecido";
        this.anoPublicacao = 0;
    }

    public Livro(String titulo, String autor, int anoPublicacao) {
        this.titulo = titulo;
        this.autor = autor;
        this.anoPublicacao = anoPublicacao;
    }

    public void imprimirDados() {
        System.out.println("Título: " + titulo);
        System.out.println("Autor: " + autor);
        System.out.println("Ano de Publicação: " + anoPublicacao);
        System.out.println("------------------------");
    }
}
