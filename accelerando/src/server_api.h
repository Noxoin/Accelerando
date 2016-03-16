
class User {
    public:
        User(char * username, char * password);
        ~User();
        void recordScore(int song_id, int score, float accuracy);
        bool isLoggedIn();
    private:
        char * token;
        char * username;
};
