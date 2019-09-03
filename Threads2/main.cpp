#include <cstdlib>
#include <iostream>
#include <random>
#include <ctime>
#include <memory>
#include <iomanip>
#include <chrono>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <future>


class Matrix {
public:
    explicit Matrix(size_t N) : N(N) {
        data = std::vector<double>(N*N);
    }

    Matrix(const Matrix &source) : Matrix(source.N) {
    }

    Matrix& operator=(const Matrix &source) {
        if (&source != this) {
            data = source.data;
            N = source.N;
        }
        return *this;
    }

    static Matrix Random (size_t N, double min, double max) {
        std::random_device rdev;
        std::mt19937 gen(rdev());
        gen.discard(N);

        std::uniform_real_distribution<double> rndgen(min, max);

        Matrix res(N);

        for(size_t i = 0; i < N*N; i++) {
            res.data[(i / N) * N + i % N] = rndgen(gen); // gen() % 3 + 1;
        }

        return res;
    }

    Matrix operator *(const Matrix &b) const {
        Matrix res(N);

        for(size_t i = 0; i < N; i++) {
            for(size_t j = 0; j < N; j++) {
                double s = 0;
                for(size_t k = 0; k < N; k++) {
                    s += data[i*N + k] * b.data[k*N + j];
                }
                res.data[i*N + j] = s;
            }
        }
        return res;
    }

    Matrix fastMult(const Matrix &b, size_t parts) const {
        Matrix res(N);
        std::vector<std::thread> th(parts);
        size_t partSize = N / parts;
        for (int i = 0; i < parts - 1; ++i)
            th[i] = std::thread(blockMult, std::ref(*this), 
                std::ref(b), i * partSize, (i + 1) * partSize, std::ref(res));

        blockMult(*this, b,  (parts - 1) * partSize, N, res);

        for (int i = 0; i < parts - 1; ++i)
            th[i].join();
        return res;
    }

    bool operator ==(const Matrix &b) const {
        for(size_t i = 0; i < N*N; i++) {
            if(std::fabs(data[i] - b.data[i]) > std::numeric_limits<double>::epsilon()) return false;
        }
        return true;
    }

    bool operator !=(const Matrix &b) const {
        return !operator==(b);
    }

    friend std::ostream &operator<<(std::ostream &out, const Matrix & m) {
        for(size_t i = 0; i<m.N*m.N; i++) {
            out << std::fixed << std::setprecision(1) << m.data[(i / m.N)*m.N + i % m.N] << ((i+1) % m.N ? " " : "\n");
        }
        return out;
    }



private:
    size_t N;
    std::vector<double> data;
    std::mutex data_lock;
    std::shared_mutex log_lock;
    void static blockMult(const Matrix &a, const Matrix&b, size_t start, size_t end, Matrix &res) {
        size_t N = a.N;
        for(size_t i = start; i < end; i++) {
            for(size_t j = 0; j < N; j++) {
                double s = 0;
                for(size_t k = 0; k < N; k++) {
                    s += a.data[i*N + k] * b.data[k*N + j];
                }
                //std::lock_guard<std::mutex>(res.data_lock);
                //std::lock_guard<std::mutex>(res.log_lock);
                std::lock(res.data_lock, res.log_lock);

                res.data[i*N + j] = s;
            }
        }
    }
};

int main() {
    const size_t N = 800;
    const int tests = 5;
    const unsigned threads = 4;
    double total1 = 0;
    double total2 = 0;
    Matrix a = Matrix::Random(N, 0, 10);
    Matrix b = Matrix::Random(N, 0, 10);
    std::chrono::high_resolution_clock timer;

    for (int i = 0; i < tests; ++i){
    auto start = timer.now();
    Matrix c = a * b;
    auto end = timer.now();
    std::cout << "Plain mult: " << std::chrono::duration<double>(end - start).count() <<std::endl;
    total1 += std::chrono::duration<double>(end - start).count();
    start = timer.now();
    Matrix d = a.fastMult(b, threads);
    end = timer.now();
    total2 += std::chrono::duration<double>(end - start).count();
    std::cout << "Multithreaded mult: " << std::chrono::duration<double>(end - start).count() <<std::endl;
    
    if(c != d) {
        std::cout <<std::endl << "Fast mult failed!!!" <<std::endl;
    }
    else {

    }

    }

    std::cout << std::endl << "MultiThread is faster in: " << total1 / total2 <<std::endl;
    

    return 0;
}