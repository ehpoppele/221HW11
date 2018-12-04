#include "hgrex_chromosome.hh"

#include <cassert>
#include <chrono>
#include <random>
#include <algorithm>

std::pair<Chromosome*, Chromosome*> HgrexChromosome::recombine(const HgrexChromosome* other)
{
    assert(is_valid());
    assert(other->is_valid());

  // Make children:
    auto child1 = hgrex_crossover_child(this, other);
    auto child2 = hgrex_crossover_child(other, this);
    return std::make_pair(child1, child2);
}

unsigned HgrexChromosome::find_index(unsigned x) const
{
    assert(is_valid());
    //set i equal to index of where x is in the chromosome
    for (unsigned i = 0; i < order_.size(); i++)
    {
        if (order_.at(i) == x)
        {
            return i;
        }
    }
    assert(false);//This would mean that the chromosome does not contain x;
    //given we have asserted the chromosome is valid, this will only happen
    //if we called this method in the wrong way, so the solution to is 
    //just break everything. Fortunately this should never happen.
}

//Returns a random unsigned integer between 0 and chromosome size-1 that is not already in the given range.
//Note that this is also an exclusive range currently.
//We don't check for validity because this is occuring while the child is still a mutant monster
//By that I mean that given the nature of recombine, it is almost certainly not valid at the time this method is called.
unsigned HgrexChromosome::random_unused(unsigned begin, unsigned end) const
{
    Cities::permutation_t holder = {};//empty vector
    for (unsigned x = 0; x < order_.size(); x++)
    {
        if(std::find((order_.begin() + begin), (order_.begin() + end), x) != (order_.begin() + end)) 
        {
            //then the element is already there, so we do nothing
        } else {
            holder.push_back(x);
        }
    }
    //now we just pick an element in holder at random
    unsigned int range = holder.size() - 1;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    std::uniform_int_distribution<int> distribution(0, range);
    return holder.at(distribution(rng));
}

//Creates the crossover child, following the HGreX crossover described in the article.
HgrexChromosome* HgrexChromosome::hgrex_crossover_child(const HgrexChromosome* p1, const HgrexChromosome* p2) const
{
    //first check the sizes are equal, and create the initial child
    const unsigned len = p1->order_.size();
    assert(len == p2->order_.size());
    HgrexChromosome* child = p1->clone();
    
    //And now we begin recombining
    //To ensure we end up with two different children by calling this
    //twice with flipped arguments, we always default to the first parent
    //for the first arc rather than use the shorter arc.
    child->order_[0] = p1->order_[0];
    child->order_[1] = p1->order_[1];
    unsigned i = 1; //starting index for the arc copying
    //Now we loop through the rest of the child. For each element, we find that element in each parent, and see wherex
    //each parent goes to from there. We then compare which of those paths is shorter, and use that one. If a path would
    //close the loop too soon, we default to the other parent, and if not possible, pick at random.
    while (i < p1->order_.size() -1 )//Using less than -1 for loop since each loop fills the element at index i + 1
    {
        unsigned p1_index = p1->find_index(child->order_.at(i));
        unsigned p2_index = p2->find_index(child->order_.at(i));
        //the next pair of lines is quite convoluted. It finds the distance between the city element we're looking at (child->order_.at(i)),
        //and the element that each parent respectively goes to next after that first one
        //%size appears frequently to insure we don't go out of range
        double p1_dist = p1->cities_ptr_->dist_between(cities_ptr_->get_elements().at(p1->order_.at(p1_index)), cities_ptr_->get_elements().at( p1->order_.at((p1_index + 1) % p1->order_.size() )));
        double p2_dist = p2->cities_ptr_->dist_between(cities_ptr_->get_elements().at(p2->order_.at(p2_index)), cities_ptr_->get_elements().at( p2->order_.at((p2_index + 1) % p2->order_.size() )));
        if (p1_dist < p2_dist)
        {
            //default to the shorter range
            //make sure the new element isn't already in there
            if(child->is_in_range(p1->order_.at((p1_index + 1)%p1->order_.size()), 0, i+1))
            {
                //if it is, we use the second parent instead
                //and also make sure it's not also already in our range
                if(child->is_in_range(p2->order_.at((p2_index + 1)%p2->order_.size()), 0, i+1))
                {
                    //Nothing works, so we default to random.
                    child->order_[i+1] = child->random_unused(0, i + 1);
                } else {
                child->order_[i+1] = p2->order_[(p2_index + 1)%p2->order_.size()];
                }
            } else {
            child->order_[i+1] = p1->order_[(p1_index + 1)%p1->order_.size()];
            }
        } else {
            //Use parent 2
             //make sure the new element isn't already in there
            if(child->is_in_range(p2->order_.at((p2_index + 1)%p2->order_.size()), 0, i+1))
            {
                //if it is, we use the first parent instead
                //and also make sure it's not also already in our range
                if(child->is_in_range(p1->order_.at((p1_index + 1)%p1->order_.size()), 0, i+1))
                {
                    //Nothing works, so we default to random.
                    child->order_[i+1] = child->random_unused(0, i + 1);
                } else {
                child->order_[i+1] = p1->order_[(p1_index + 1)%p1->order_.size()];
                }
            } else {
            child->order_[i+1] = p2->order_[(p2_index + 1)%p2->order_.size()];
            }
        }
        i++;
    }
    
    //Then return
    //We can't assert validity until this point.
    assert(child->is_valid());
    return child;
}




